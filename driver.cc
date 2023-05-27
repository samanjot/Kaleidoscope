#include "driver.hh"
#include "parser.hh"

static AllocaInst *CreateEntryBlockAlloca(driver &drv,
                                          Function *func,
                                          const std::string &name,
                                          Type *type) 
{
  IRBuilder<> tmp(&func->getEntryBlock(), 
                  func->getEntryBlock().begin());

  return tmp.CreateAlloca(type, nullptr, name);
}

Value *LogErrorV(const std::string Str) {
  std::cerr << Str << std::endl;
  return nullptr;
}

static Type* GetDoubleArrayType(driver &drv, uint64_t size)
{
  return ArrayType::get(Type::getDoubleTy(*drv.context), size);
}

static Value* GetArrayAddress(driver &drv, Value* idx, Value* base)
{
  if(!base->getType()->isPointerTy()) {
    drv.CodegenError("Errore: non è consentito l'accesso ad un elemento per il tipo double\n");
    return nullptr;
  }

  if(!idx->getType()->isDoubleTy()) {
    drv.CodegenError("Errore: non è consentito l'accesso ad un elemento dove l'indice non è di tipo double\n");
  }

  Type* baseType = static_cast<PointerType*>(base->getType());
  baseType = baseType->getContainedType(0);

  // A seconda che si utilizzino gli array o i puntatori, si fa un utilizzo leggermente
  // diverso dell'istruzione GetElementPointer.
  if(baseType->isArrayTy()) {  // Puntatore ad un array
    Value* baseAddr = drv.builder->CreateLoad(baseType, base, "loadtmp");
    Value* idxInt = drv.builder->CreateFPToUI(idx, Type::getInt64Ty(*drv.context), "casttmp");

    Value* indices[3] = { ConstantInt::get(*drv.context, APInt(32, 0)), idxInt };
    ArrayRef<Value*> valueArr(indices, 2);
    
    return drv.builder->CreateInBoundsGEP(baseType, base, valueArr, "GEPtmp");
  }
  else if(baseType == Type::getDoublePtrTy(*drv.context)) {  // Puntatore ad un puntatore
    Value* baseAddr = drv.builder->CreateLoad(Type::getDoublePtrTy(*drv.context), base, "loadtmp");
    Value* idxInt = drv.builder->CreateFPToUI(idx, Type::getInt64Ty(*drv.context), "casttmp");
    return drv.builder->CreateGEP(Type::getDoubleTy(*drv.context), baseAddr, idxInt, "GEPtmp");
  }
  else {
    if(baseType->isDoubleTy())
      drv.CodegenError("Errore: Impossibile accedere ad un elemento di una variabile di tipo double\n");
    else
      drv.CodegenError("Errore: Tipo sconosciuto per accesso ad un elemento\n");

    return nullptr;
  }
}

// Funzione per il caso in cui idx sia noto e costante (ad esempio per le store degli
// elementi degli array nella varexpr)
static Value* GetArrayAddress(driver &drv, uint64_t idx, Value* base)
{
  if(!base->getType()->isPointerTy()) {
    drv.CodegenError("Errore: non è consentito l'accesso ad un elemento per il tipo double\n");
    return nullptr;
  }

  Type* baseType = static_cast<PointerType*>(base->getType());
  baseType = baseType->getContainedType(0);

  // A seconda che si utilizzino gli array o i puntatori, si fa un utilizzo leggermente
  // diverso dell'istruzione GetElementPointer.
  if(baseType->isArrayTy()) {  // Puntatore ad un array
    Value* baseAddr = drv.builder->CreateLoad(baseType, base, "loadtmp");

    Value* indices[3] = { ConstantInt::get(*drv.context, APInt(32, 0)), ConstantInt::get(*drv.context, APInt(64, idx)) };
    ArrayRef<Value*> valueArr(indices, 2);

    return drv.builder->CreateInBoundsGEP(baseType, base, valueArr, "GEPtmp");
  }
  else  // Puntatore ad un puntatore
    return drv.builder->CreateLoad(Type::getDoublePtrTy(*drv.context), base, "loadtmp");
}

/*************************** Driver class *************************/
driver::driver(): trace_parsing (false), trace_scanning (false), ast_print (false) {
  context = new LLVMContext;
  module = new Module("Kaleidoscope", *context);
  builder = new IRBuilder(*context);
};

int driver::parse (const std::string &f) {
  file = f;
  location.initialize(&file);
  scan_begin();
  yy::parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_end();
  return res;
}

void driver::codegen() {
  if (ast_print) root->visit();
  std::cout << std::endl;
  root->codegen(*this);
};

/********************** Handle Top Expressions ********************/
Value* TopExpression(ExprAST* E, driver& drv) {
  // Crea una funzione anonima anonima il cui body è un'espressione top-level
  // viene "racchiusa" un'espressione top-level
  E->toggle(); // Evita la doppia emissione del prototipo
  PrototypeAST *Proto = new PrototypeAST("__espr_anonima"+std::to_string(++drv.Cnt),
		  std::vector<ProtoArgument>());
  Proto->noemit();
  FunctionAST *F = new FunctionAST(std::move(Proto),E);
  auto *FnIR = F->codegen(drv);
  FnIR->eraseFromParent();
  return nullptr;
};

/************************ Expression tree *************************/
  // Inverte il flag che definisce le TopLevelExpression
  // ando viene chiamata
void ExprAST::toggle() {
  top = top ? false : true;
};

bool ExprAST::gettop() {
  return top;
};

/************************* Sequence tree **************************/
SeqAST::SeqAST(RootAST* first, RootAST* continuation):
  first(first), continuation(continuation) {};

void SeqAST:: visit() {
  if (first != nullptr) {
    first->visit();
  } else {
    if (continuation == nullptr) {
      return;
    };
  };
  std::cout << ";";
  continuation->visit();
};

Value *SeqAST::codegen(driver& drv) {
  Value* f = nullptr;
  Value* c = nullptr;
  
  if(first)
  {
    f = first->codegen(drv);
    if(!f) {
      std::cerr << drv.error_string;
      drv.error_string = std::string();
      return nullptr;
    }
  }

  if(continuation)
  {
    c = continuation->codegen(drv);
    if(!c)
      return nullptr;
  }

  return f;
};

/********************* Number Expression Tree *********************/
NumberExprAST::NumberExprAST(double Val): Val(Val) { top = false; };
void NumberExprAST::visit() {
  std::cout << Val << " ";
};

double NumberExprAST::GetVal() { return Val; }

Value *NumberExprAST::codegen(driver& drv) {  
  if (gettop()) return TopExpression(this, drv);
  else return ConstantFP::get(*drv.context, APFloat(Val));
};

/****************** Variable Expression TreeAST *******************/
VariableExprAST::VariableExprAST(std::string &Name):
  Name(Name) { top = false; };
VariableExprAST::VariableExprAST(std::string &Name, ExprAST* idx)
: Name(Name), idx(idx) { top = false; };

const std::string& VariableExprAST::getName() const {
  return Name;
};

void VariableExprAST::visit() {
  std::cout << getName() << " ";
};

Value *VariableExprAST::codegen(driver& drv) {
  if (gettop())
    return TopExpression(this, drv);

  Value* result = nullptr;
  AllocaInst *V = drv.NamedValues[Name];
  if (!V) LogErrorV("Variabile non definita");
  if(V->getType()->isPointerTy()) {
    Type* baseType = static_cast<PointerType*>(V->getType());
    baseType = baseType->getContainedType(0);
    if(!baseType->isDoubleTy()) {  // Si tratta di un array (tipo puntatore oppure array)
      if(idx) {
        Value* idxValue = idx->codegen(drv);
        if(!idxValue)
          return nullptr;
        
        Value *arrAddr = GetArrayAddress(drv, idxValue, V);
        if(!arrAddr)
          return nullptr;

        result = drv.builder->CreateLoad(Type::getDoubleTy(*drv.context), arrAddr, "loadtmp");
      }
      else {
        return GetArrayAddress(drv, (uint64_t)0, V);
      }
    } else {
      result = drv.builder->CreateLoad(V->getAllocatedType(), V, Name.c_str());
    }
  }
  return result;
};

/********************* Unary Expression Tree ***********************/
UnaryExprAST::UnaryExprAST(char op, ExprAST *expr) :
  op{op}, expr{expr} { }

void UnaryExprAST::visit() 
{
  std::cout << '(' << op;
  expr->visit();
  std::cout << ')';
}

Value* UnaryExprAST::codegen(driver& drv) {
  if (gettop())
    return TopExpression(this, drv);
  
  Value* value = expr->codegen(drv);
  if (!value) return nullptr;
  
  switch (op) {
  case '+':
    return value;
  case '-':
    return drv.builder->CreateFSub(ConstantFP::get(*drv.context, APFloat(0.0)), value);
  default:  
    return LogErrorV("Operatore unario non supportato");
  }
}

/********************* Assignment Expression Tree ***********************/
AssignmentExprAST::AssignmentExprAST(std::string id, ExprAST* expr, ExprAST* idx)
: id(id), expr(expr), idx(idx) {}

void AssignmentExprAST::visit() 
{
  std::cout << '(' << id << " = ";
  expr->visit();
  std::cout << ')';
}

Value* AssignmentExprAST::codegen(driver& drv) 
{
  Value* value = nullptr;
  if(expr) {
    value = expr->codegen(drv);
    if (!value)
      return nullptr;
  }

  // Il rhs deve per forza essere un double
  if(!value->getType()->isDoubleTy()) {
    drv.CodegenError("Errore: Impossibile assegnare ad una variabile un valore di tipo 'array'\n");
    return nullptr;
  }

  Value *var = drv.NamedValues[id];
  if (!var)
    return nullptr;

  if (idx) {
    Value* idxValue = idx->codegen(drv);
    if(!idxValue)
      return nullptr;

    // In questo modo, la store viene effettuata nel singolo elemento dell'array
    var = GetArrayAddress(drv, idxValue, var);
  }
  else {
    if(var->getType() != Type::getDoublePtrTy(*drv.context)) {
      drv.CodegenError("Errore: Assegnamento di un array non consentito\n");
      return nullptr;
    }
  }

  if(value)
    drv.builder->CreateStore(value, var);
  return value;
}

/******************** Binary Expression Tree **********************/
BinaryExprAST::BinaryExprAST(BinOp Op, ExprAST* LHS, ExprAST* RHS):
  Op(Op), LHS(LHS), RHS(RHS) { top = false; };
 
void BinaryExprAST::visit() {
  std::cout << "(" << Op << " ";
  LHS->visit();
  if (RHS!=nullptr) RHS->visit();
  std::cout << ")";
};

Value *BinaryExprAST::codegen(driver& drv) {
  if (gettop()) {
    return TopExpression(this, drv);
  } else {
    Value *L = LHS->codegen(drv);
    Value *R = RHS->codegen(drv);
    if (!L || !R) return nullptr;
    
    // Soltanto operazioni tra double sono ammesse, tranne per le operazioni di confronto
    if(!L->getType()->isDoubleTy() || !R->getType()->isDoubleTy()) {
      drv.CodegenError("Errore: Impossibile effettuare un'operazione binaria che non coinvolga due double\n");
      return nullptr;
    }
    
    switch (Op) {
    case Op_Add:
      return drv.builder->CreateFAdd(L,R,"addregister");
    case Op_Sub:
      return drv.builder->CreateFSub(L,R,"subregister");
    case Op_Mul:
      return drv.builder->CreateFMul(L,R,"mulregister");
    case Op_Div:
      return drv.builder->CreateFDiv(L,R,"addregister");
    case Op_GT:
      return drv.builder->CreateFCmpOGT(L,R,"cmpregister");
    case Op_GE:
      return drv.builder->CreateFCmpOGE(L,R,"cmpregister");
    case Op_LT:
      return drv.builder->CreateFCmpOLT(L,R,"cmpregister");
    case Op_LE:
      return drv.builder->CreateFCmpOLE(L,R,"cmpregister");
    case Op_NE:
      return drv.builder->CreateFCmpONE(L,R,"cmpregister");
    case Op_MultiExp:
      return R;
    default:
      return LogErrorV("Operatore binario non supportato");
    }
  }
};

/********************* Call Expression Tree ***********************/
CallExprAST::CallExprAST(std::string Callee, std::vector<ExprAST*> Args): Callee(Callee),
									  Args(std::move(Args)) { top = false; };
void CallExprAST::visit() {
  std::cout << Callee << "( ";
  for (ExprAST* arg : Args) {
    arg->visit();
  };
  std::cout << ')';
};

Value *CallExprAST::codegen(driver& drv) {
  if (gettop()) {
    return TopExpression(this, drv);
  } else {
    // Cerchiamo la funzione nell'ambiente globale
    Function *CalleeF = drv.module->getFunction(Callee);
    if (!CalleeF)
      return LogErrorV("Funzione non definita\n");
    // Controlliamo che gli argomenti coincidano in numero coi parametri

    bool sameNumArgs = (Args.size() == 1 && !Args[0] && CalleeF->arg_size() == 0);  // Viene inserito nullptr dentro al vector
    sameNumArgs |= CalleeF->arg_size() == Args.size();
    if (!sameNumArgs) {
      drv.CodegenError("Errore: il numero di argomenti della funzione non coincide con quelli della chiamata\n");
      std::cout << CalleeF->arg_size() << " " << Args.size() << "\n"; 
      return nullptr;
    }

std::cout << CalleeF->arg_size() << " " << Args.size() << "\n";

    std::vector<Value *> ArgsV;
    for(int i = 0; i < Args.size(); ++i) {
      if(!Args[i]) continue;  // Il primo elemento è nullptr quando il vettore dovrebbe essere vuoto

      Value* argVal = Args[i]->codegen(drv);
      if(CalleeF->getArg(i)->getType() != argVal->getType()) {
        drv.CodegenError("Errore: i tipi degli argomenti della funzione non coincidono con quelli della chiamata\n");
        return nullptr;
      }

      ArgsV.push_back(argVal);

      if (!ArgsV.back())
	      return nullptr;
    }

    return drv.builder->CreateCall(CalleeF, ArgsV, "calltmp");
  }
}

/************************* Prototype Tree *************************/
PrototypeAST::PrototypeAST(std::string Name, std::vector<ProtoArgument> Args): Name(Name),
									     Args(std::move(Args)) { emit = true; };
const std::string& PrototypeAST::getName() const { return Name; };
const std::vector<ProtoArgument>& PrototypeAST::getArgs() const { return Args; };
void PrototypeAST::visit() {
  std::cout << "extern " << getName() << "( ";
  
  for (ProtoArgument arg : Args){
    std::cout << arg.name;
    if (arg.isPointer)
      std::cout << "[]";

    std::cout << " ";
  }
  
  std::cout << ')';
};

void PrototypeAST::noemit() { emit = false; };

bool PrototypeAST::emitp() { return emit; };

Function *PrototypeAST::codegen(driver& drv) {
  // Costruisce una struttura double(double,...,double) che descrive 
  // tipo di ritorno e tipo dei parametri (in Kaleidoscope solo double)
  std::vector<Type*> Doubles;
  //auto *doubleT = Type::getDoubleTy(*drv.context);

  for (ProtoArgument arg : Args){
    Doubles.push_back((arg.isPointer) ? Type::getDoublePtrTy(*drv.context)
                                      : Type::getDoubleTy(*drv.context));
  }
 
  FunctionType *FT =
      FunctionType::get(Type::getDoubleTy(*drv.context), Doubles, false);
  Function *F =
      Function::Create(FT, Function::ExternalLinkage, Name, *drv.module);
  if(!F)
    return nullptr;

  // Attribuiamo agli argomenti il nome dei parametri formali specificati dal programmatore
  unsigned Idx = 0;
  for (auto &Arg : F->args())
    Arg.setName(Args[Idx++].name);

  if (emitp()) {  // emitp() restituisce true se e solo se il prototipo è definito extern
    F->print(errs());
    fprintf(stderr, "\n");
  };
  
  return F;
}

/************************* Function Tree **************************/
FunctionAST::FunctionAST(PrototypeAST* Proto, ExprAST* Body):
  Proto(Proto), Body(Body) {
  if (Body == nullptr) external=true;
  else external=false;
};

void FunctionAST::visit() {
  std::cout << Proto->getName() << "( ";
  for (ProtoArgument arg : Proto->getArgs()){
    std::cout << arg.name;
    if (arg.isPointer)
      std::cout << "[]";

    std::cout << " ";
  }
  
  std::cout << ')';
  Body->visit();
};

Function *FunctionAST::codegen(driver& drv) {
  // Verifica che non esiste già, nel contesto, una funzione con lo stesso nome
  std::string name = Proto->getName();
  Function *TheFunction = drv.module->getFunction(name);
  // E se non esiste prova a definirla
  if (TheFunction) {
    LogErrorV("Funzione "+name+" già definita");
    return nullptr;
  }
  if (!TheFunction)
    TheFunction = Proto->codegen(drv);
  if (!TheFunction)
    return nullptr;

  // Crea un blocco di base in cui iniziare a inserire il codice
  BasicBlock *BB = BasicBlock::Create(*drv.context, "entry", TheFunction);
  drv.builder->SetInsertPoint(BB);
  
  // Registra gli argomenti nella symbol table
  drv.NamedValues.clear();
  for (auto &Arg : TheFunction->args()) {
    AllocaInst *alloca = CreateEntryBlockAlloca(drv, TheFunction, std::string(Arg.getName()),
                                                Arg.getType());
    drv.builder->CreateStore(&Arg, alloca);
    drv.NamedValues[std::string(Arg.getName())] = alloca;
  }

  Value* retVal = Body->codegen(drv);
  bool error = false;
  if(!retVal)
  {
    // Ritorna 0 se qualcosa è andato storto
    retVal = ConstantFP::get(*drv.context, APFloat(0.0));
    error = true;
  }

  drv.builder->CreateRet(retVal);

  TheFunction->print(errs());

  raw_os_ostream ostream(std::cout);
  std::cerr << '\n';
  if(verifyFunction(*TheFunction, &ostream)) {
    std::cerr << "\nErrore: Funzione malformata, guardare i dettagli sopra.\n";
    drv.codegen_error = true;

    // Funzione malformata
    TheFunction->eraseFromParent();
    return nullptr;
  }

  // Errore nel codegen del body
  if(error) {
    std::cerr << "è avvenuto un errore nel codegen del body della funzione.\n";
    drv.codegen_error = true;
    
    TheFunction->eraseFromParent();
    return nullptr;
  }

  return TheFunction;
}

/************************* If Expression Tree **************************/

IfExprAST::IfExprAST(ExprAST* cond, ExprAST* thenExpr, ExprAST* elseExpr) {
  this->cond = cond;
  this->thenExpr = thenExpr;
  this->elseExpr = elseExpr;
}

void IfExprAST::visit() {
  std::cout << "if (";
  cond->visit();
  std::cout << ") then (";
  thenExpr->visit();
  std::cout << ") else (";
  elseExpr->visit();
  std::cout << '\n';
}

Value* IfExprAST::codegen(driver& drv) {
  if(gettop())
    return TopExpression(this, drv);

  Value* condValue = cond->codegen(drv);
  if (!condValue)
    return nullptr;

  // Solo value di tipo bool (cioè int1) oppure eventualmente double solo consentiti
  if(condValue->getType()->isDoubleTy())
    condValue = drv.builder->CreateFCmpONE(condValue, ConstantFP::get(*drv.context, APFloat(0.0)), "ifcond");
  else if(!condValue->getType()->isIntegerTy(1)) {  // Non è un bool
    drv.CodegenError("Errore: condizione dell'if consente solamente bool o double\n");
    return nullptr;
  }

  Function* curFunction = drv.builder->GetInsertBlock()->getParent();

  BasicBlock* thenBB  = BasicBlock::Create(*drv.context, "then", curFunction);
  BasicBlock* elseBB  = BasicBlock::Create(*drv.context, "else", curFunction);
  BasicBlock* mergeBB = BasicBlock::Create(*drv.context, "ifcont", curFunction);

  drv.builder->CreateCondBr(condValue, thenBB, elseBB);

  // Generazione del blocco 'then'
  drv.builder->SetInsertPoint(thenBB);
  Value* thenValue = thenExpr->codegen(drv);
  if (!thenValue)
    return nullptr;

  drv.builder->CreateBr(mergeBB);
  //thenBB = drv.builder->GetInsertBlock();

  // Generazione del blocco 'else'
  drv.builder->SetInsertPoint(elseBB);

  Value* elseValue = elseExpr->codegen(drv);
  if (!elseValue)
    return nullptr;

  drv.builder->CreateBr(mergeBB);

  // Generazione del blocco 'merge'
  drv.builder->SetInsertPoint(mergeBB);

  PHINode* phi = drv.builder->CreatePHI(Type::getDoubleTy(*drv.context), 2, "iftmp");
  phi->addIncoming(thenValue, thenBB);
  phi->addIncoming(elseValue, elseBB);
  return phi;
}

/************************* For Expression Tree **************************/

ForExprAST::ForExprAST(std::string id, ExprAST* init, ExprAST* cond, ExprAST* step, ExprAST *body) 
: id(id), init(init), cond(cond), step(step), body(body)
{
  
}

void ForExprAST::visit() {
  std::cout << "for " << id << " = (";
  init->visit();
  std::cout << "), (";
  cond->visit();
  if(step) {
    std::cout << ") step (";
    step->visit();
  } else
    std::cout << ") step (1";
  std::cout << ") in (";
  body->visit();
  std::cout << ")\n";
}

Value *ForExprAST::codegen(driver& drv)
{
  if (gettop()) {
    return TopExpression(this, drv);
  }

  BasicBlock *preheader = drv.builder->GetInsertBlock();
  Function* curFunction = preheader->getParent();

  AllocaInst *alloca = CreateEntryBlockAlloca(drv, curFunction, id,
                                              Type::getDoubleTy(*drv.context));

  Value *initVal = init->codegen(drv);
  if (!initVal)
    return nullptr;

  BasicBlock *header = BasicBlock::Create(*drv.context, "header", curFunction);
  BasicBlock *bodyBB = BasicBlock::Create(*drv.context, "body", curFunction);
  BasicBlock *exitBB = BasicBlock::Create(*drv.context, "exit", curFunction);

  drv.builder->CreateBr(header);

  // Generazione blocco 'header'
  drv.builder->SetInsertPoint(header);

  PHINode* phi = drv.builder->CreatePHI(Type::getDoubleTy(*drv.context), 2, "fortmp");
  phi->addIncoming(ConstantFP::get(*drv.context, APFloat(0.0)),
                   preheader);

  // Per lo shadowing della variabile dell'iterazione
  AllocaInst *oldVal = drv.NamedValues[id];
  drv.NamedValues[id] = alloca;

  Value* condValue = cond->codegen(drv);
  if(!condValue)
    return nullptr;
  // Solo value di tipo bool (cioè int1) oppure eventualmente double solo consentiti
  if(condValue->getType()->isDoubleTy())
    condValue = drv.builder->CreateFCmpONE(condValue, ConstantFP::get(*drv.context, APFloat(0.0)), "forcond");
  else if(!condValue->getType()->isIntegerTy(1)) {  // Non è un bool
    drv.CodegenError("Errore: condizione del for consente solamente bool o double\n");
    return nullptr;
  }
  
  drv.builder->CreateCondBr(condValue, bodyBB, exitBB);
  drv.builder->SetInsertPoint(exitBB);

  // Generazione del blocco 'body'
  drv.builder->SetInsertPoint(bodyBB);
  auto bodyValue = this->body->codegen(drv);
  if(!bodyValue)
    return nullptr;

  // Il codegen di body potrebbe nel frattempo aver creato altri blocchi
  // e/o aver alterato il blocco di inserimento del builder
  BasicBlock* bodyExitBB = drv.builder->GetInsertBlock();

  Value* stepValue;
  if(step)
    stepValue = step->codegen(drv);
  else
    stepValue = ConstantFP::get(*drv.context, APFloat(1.0));

  if(!stepValue)
    return nullptr;

  // Solo value di tipo double sono consentiti
  if(!stepValue->getType()->isDoubleTy()) {
    drv.CodegenError("Errore: lo step del for consente solamente valori di tipo double\n");
    return nullptr;
  }

  Value *currentValue = drv.builder->CreateLoad(alloca->getAllocatedType(), alloca, id.c_str());
  stepValue = drv.builder->CreateFAdd(currentValue, stepValue, "steptmp");
  drv.builder->CreateStore(stepValue, alloca);

  phi->addIncoming(bodyValue, bodyExitBB);
  drv.builder->CreateBr(header);

  // Ripristino della vecchia variabile
  drv.NamedValues[id] = oldVal;
  drv.builder->SetInsertPoint(exitBB);

  return phi;
}

/************************* Varexpr Expression Tree **************************/

VarExprAST::VarExprAST(std::vector<Pair>& vars, ExprAST* body)
: vars(vars), body(body)
{ }

void VarExprAST::visit() {
  std::cout << "varexpr list (";
  for(int i = 0; i < vars.size(); ++i) {
    vars[i].expr->visit();
    if(i < vars.size() - 1)
      std::cout << " , ";
  }

  std::cout << ") body (";
  body->visit();
  std::cout << ") ";
}

Value* VarExprAST::codegen(driver& drv) {
  if(gettop())
    return TopExpression(this, drv);

  // Per gestire lo shadowing
  std::map<std::string, AllocaInst*> shadowed = drv.NamedValues;

  // Passata per le allocazioni stack
  for(int i = 0; i < vars.size(); ++i) {
    //// Allocazione ////
    AllocaInst* alloca = nullptr;
    bool isVLA = false;
    uint64_t staticLength = 0;

    if(vars[i].arrSize) {
      if(auto numExpr = dynamic_cast<NumberExprAST*>(vars[i].arrSize)) {
        staticLength = (uint64_t)numExpr->GetVal();
        alloca = CreateEntryBlockAlloca(drv, drv.builder->GetInsertBlock()->getParent(), vars[i].id,
                                        ArrayType::get(Type::getDoubleTy(*drv.context), staticLength));
      }
      else {  // Variable length array
        isVLA = true;
        Value* arrLength = vars[i].arrSize->codegen(drv);
        if(!arrLength)
          return nullptr;

        if(!arrLength->getType()->isDoubleTy()) {
          drv.CodegenError("Errore: La dimensione di un array deve essere di tipo double\n");
          return nullptr;
        }

        // Conversione in int
        Value* arrLengthInt = drv.builder->CreateFPToUI(arrLength, Type::getInt64Ty(*drv.context), "casttmp");

        // Molto meno probabile che l'ottimizzatore la possa rimuovere (essendo VLA),
        // quindi si può inserire direttamente qua
        alloca = drv.builder->CreateAlloca(Type::getDoubleTy(*drv.context), arrLengthInt);
      }
    }
    else
      alloca = CreateEntryBlockAlloca(drv, drv.builder->GetInsertBlock()->getParent(), vars[i].id, Type::getDoubleTy(*drv.context));

    //// Inizializzazione ////
    if(vars[i].arrSize) {  // Si tratta di una dichiarazione di array

      if(vars[i].expr) {
        drv.CodegenError("Errore: un array deve essere inizializzato tramite initializer list\n");
        return nullptr;
      }

      if(isVLA && vars[i].initList.size() > 0) {
        drv.CodegenError("Errore: non è consentito inizializzare un VLA staticamente\n");
        return nullptr;
      }
      
      if(!isVLA && vars[i].initList.size() > 0 && staticLength != vars[i].initList.size()) {
        drv.CodegenError("Errore: la dimensione dell'array dichiarato non combacia con il numero di elementi nell'initializer list\n");
        return nullptr;
      }

      for(int j = 0; j < vars[i].initList.size(); ++j) {

        Value* toStore = vars[i].initList[j]->codegen(drv);
        Value* storeInto = GetArrayAddress(drv, j, alloca);
        if(!toStore || !storeInto)
          return nullptr;

        if(!toStore->getType()->isDoubleTy()) {
          drv.CodegenError("Errore: Soltanto valori di tipo double sono ammessi in un initializer list\n");
          return nullptr;
        }

        drv.builder->CreateStore(toStore, storeInto);
      }
    }
    else if(vars[i].expr) {  // Si tratta di una dichiarazione di double
      Type* allocatedType = static_cast<PointerType*>(alloca->getType());
      allocatedType = allocatedType->getContainedType(0);

      Value* exprValue = vars[i].expr->codegen(drv);
      if(!exprValue) return nullptr;

      if(allocatedType != exprValue->getType()) {
        drv.CodegenError("Errore: La variabile dichiarata e il valore con cui è stata inizializzata non coincidono\n");
        return nullptr;
      }

      drv.builder->CreateStore(exprValue, alloca);
    }

    //// Memorizzazione nella symbol table ////
    drv.NamedValues[vars[i].id] = alloca;
  }

  Value *value = body->codegen(drv);
  if(!value)
    return nullptr;

  // Ripristino della mappa
  drv.NamedValues = shadowed;

  return value;
}

/************************* While Expression Tree **************************/

WhileExprAST::WhileExprAST(ExprAST* cond, ExprAST* body)
: cond(cond), body(body) { }

void WhileExprAST::visit() {
  std::cout << "while (";
  cond->visit();
  std::cout << ") in (";
  body->visit();
  std::cout << ")";
}

Value* WhileExprAST::codegen(driver& drv) {
  if (gettop())
    return TopExpression(this, drv);

  BasicBlock* preheader = drv.builder->GetInsertBlock();
  Function* curFunction = preheader->getParent();

  BasicBlock *header = BasicBlock::Create(*drv.context, "header", curFunction);
  BasicBlock *bodyBB = BasicBlock::Create(*drv.context, "body", curFunction);
  BasicBlock *exitBB = BasicBlock::Create(*drv.context, "exit", curFunction);

  drv.builder->CreateBr(header);

  // Generazione blocco 'header'
  drv.builder->SetInsertPoint(header);

  PHINode* phi = drv.builder->CreatePHI(Type::getDoubleTy(*drv.context), 2, "whiletmp");
  phi->addIncoming(ConstantFP::get(*drv.context, APFloat(0.0)), preheader);

  Value* condValue = cond->codegen(drv);
  if(!condValue)
    return nullptr;
  // Solo value di tipo bool (cioè int1) oppure eventualmente double solo consentiti
  if(condValue->getType()->isDoubleTy())
    condValue = drv.builder->CreateFCmpONE(condValue, ConstantFP::get(*drv.context, APFloat(0.0)), "whilecond");
  else if(!condValue->getType()->isIntegerTy(1)) {  // Non è un bool
    drv.CodegenError("Errore: condizione del while consente solamente bool o double\n");
    return nullptr;
  }
  
  drv.builder->CreateCondBr(condValue, bodyBB, exitBB);

  // Generazione del blocco 'body'
  drv.builder->SetInsertPoint(bodyBB);
  auto bodyValue = this->body->codegen(drv);
  if(!bodyValue)
    return nullptr;

  BasicBlock* exitBodyBB = drv.builder->GetInsertBlock();
  
  phi->addIncoming(bodyValue, exitBodyBB);
  drv.builder->CreateBr(header);

  drv.builder->SetInsertPoint(exitBB);
  return phi;
}