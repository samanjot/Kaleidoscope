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
  if (first != nullptr) {
    Value *f = first->codegen(drv);
  } else {
    if (continuation == nullptr) return nullptr;
  }
  Value *c = continuation->codegen(drv);
  return nullptr;
};

/********************* Number Expression Tree *********************/
NumberExprAST::NumberExprAST(double Val): Val(Val) { top = false; };
void NumberExprAST::visit() {
  std::cout << Val << " ";
};

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
    if(baseType->isPointerTy()) {
      if(idx) {
        Value* idxValue = idx->codegen(drv);
        
        // TODO test
        Value* baseAddr = drv.builder->CreateLoad(Type::getDoublePtrTy(*drv.context), V, "loadtmp");
        // TODO check che idx sia double e non pointer
        Value* idxInt = drv.builder->CreateFPCast(idxValue, Type::getInt64Ty(*drv.context), "casttmp");
        Value* addr = drv.builder->CreateGEP(Type::getDoubleTy(*drv.context), baseAddr, idxInt, "GEPtmp");
        result = drv.builder->CreateLoad(Type::getDoubleTy(*drv.context), addr, "loadtmp");
      }
      else {
        // TODO: Stampa errore
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
  if (gettop()) {
    return TopExpression(this, drv);
  } else {
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
}

/********************* Assignment Expression Tree ***********************/
AssignmentExprAST::AssignmentExprAST(std::string id, ExprAST* expr)
: id(id), expr(expr) {}

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

  Value *var = drv.NamedValues[id];
  if (!var)
    return nullptr;
  if (idx) {
    // TODO: controllo che la var sia un array

    Value* idxValue = idx->codegen(drv);
    if(!idxValue)
      return nullptr;

    // In questo modo, la store viene effettuata nel singolo elemento dell'array
    var = drv.builder->CreateGEP(Type::getDoubleTy(*drv.context), var, idxValue, "GEPtmp");
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
      return LogErrorV("Funzione non definita");
    // Controlliamo che gli argomenti coincidano in numero coi parametri
    if (CalleeF->arg_size() != Args.size())
      return LogErrorV("Numero di argomenti non corretto");
    std::vector<Value *> ArgsV;
    for (auto arg : Args) {
      ArgsV.push_back(arg->codegen(drv));
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

  if (Value *RetVal = Body->codegen(drv)) {
    // Termina la creazione del codice corrispondente alla funzione
    drv.builder->CreateRet(RetVal);

    // Effettua la validazione del codice e un controllo di consistenza
    verifyFunction(*TheFunction);

    TheFunction->print(errs());
    fprintf(stderr, "\n");
    return TheFunction;
  }

  // Errore nella definizione. La funzione viene rimossa
  TheFunction->eraseFromParent();
  return nullptr;
};

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

  if(condValue->getType()->isDoubleTy())
    condValue = drv.builder->CreateFCmpONE(condValue, ConstantFP::get(*drv.context, APFloat(0.0)), "ifcond");

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

  //auto *phi = drv.builder->CreatePHI(Type::getDoubleTy(*drv.context), 2, "fortmp");
  //phi->addIncoming(initVal, preheader);

  Value* condValue = cond->codegen(drv);
  if(!condValue)
    return nullptr;
  if(condValue->getType()->isDoubleTy())
    condValue = drv.builder->CreateFCmpONE(condValue, ConstantFP::get(*drv.context, APFloat(0.0)), "forcond");
  
  drv.builder->CreateCondBr(condValue, bodyBB, exitBB);
  drv.builder->SetInsertPoint(exitBB);

  // Generazione del blocco 'body'
  drv.builder->SetInsertPoint(bodyBB);
  auto bodyValue = this->body->codegen(drv);
  if(!bodyValue)
    return nullptr;

  Value* stepValue;
  if(step)
    stepValue = step->codegen(drv);
  else
    stepValue = ConstantFP::get(*drv.context, APFloat(1.0));

  Value *currentValue = drv.builder->CreateLoad(alloca->getAllocatedType(), alloca, id.c_str());
  stepValue = drv.builder->CreateFAdd(currentValue, stepValue, "steptmp");
  drv.builder->CreateStore(stepValue, alloca);

  phi->addIncoming(bodyValue, bodyBB);
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

  for(int i = 0; i < vars.size(); ++i) {
    AllocaInst* alloca = drv.builder->CreateAlloca(Type::getDoubleTy(*drv.context), nullptr, vars[i].id);
    drv.NamedValues[vars[i].id] = alloca;
  }

  for(int i = 0; i < vars.size(); ++i)
    vars[i].expr->codegen(drv);

  Value *value = body->codegen(drv);
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
  phi->addIncoming(ConstantFP::get(*drv.context, APFloat(0.0)),
                   preheader);

  Value* condValue = cond->codegen(drv);
  if(!condValue)
    return nullptr;
  if(condValue->getType()->isDoubleTy())
    condValue = drv.builder->CreateFCmpONE(condValue, ConstantFP::get(*drv.context, APFloat(0.0)), "forcond");
  
  drv.builder->CreateCondBr(condValue, bodyBB, exitBB);
  

  // Generazione del blocco 'body'
  drv.builder->SetInsertPoint(bodyBB);
  auto bodyValue = this->body->codegen(drv);
  if(!bodyValue)
    return nullptr;

  phi->addIncoming(bodyValue, bodyBB);
  drv.builder->CreateBr(header);

  drv.builder->SetInsertPoint(exitBB);
  return phi;
}

/************************* InitList Expression Tree **************************/
// TODO!!!

InitListExprAST::InitListExprAST(std::vector<ExprAST*> expr)
: exprs(expr) {}

void InitListExprAST::visit()
{
  std::cout << "{";

  bool first = true;
  for (auto *expr: exprs)
  { 
      if (first)
        std::cout << ", ";

      expr->visit();
      first = false;
  }
  
  std::cout << "}";
} 

Value *InitListExprAST::codegen(driver &drv) {
  return nullptr;
}