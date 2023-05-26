#ifndef DRIVER_HH
#define DRIVER_HH
/************ Header file per la generazione del codice oggetto *************/
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
/***************************************************************************/
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include "parser.hh"

using namespace llvm;

// Dichiarazione del prototipo yylex per Flex
// Flex va proprio a cercare YY_DECL perché
// deve espanderla (usando M4) nel punto appropriato
# define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
// Per il parser è sufficiente una forward declaration
YY_DECL;

// Classe che organizza e gestisce il processo di compilazione
class driver
{
public:
  driver();
  LLVMContext *context;
  Module *module;
  IRBuilder<> *builder;
  std::map<std::string, AllocaInst *> NamedValues;
  static inline int Cnt=0; //Contatore incrementale, per identificare registri SSA
  RootAST* root;      // A fine parsing "punta" alla radice dell'AST
  int parse (const std::string& f);
  std::string file;
  bool trace_parsing; // Abilita le tracce di debug el parser
  void scan_begin (); // Implementata nello scanner
  void scan_end ();   // Implementata nello scanner
  bool trace_scanning;// Abilita le tracce di debug nello scanner
  yy::location location; // Utillizata dallo scannar per localizzare i token
  bool ast_print;
  bool codegen_error = false;
  std::string error_string;
  void codegen();
  void CodegenError(const char* error) { codegen_error = true; error_string.append(error); }
};

// Classe base dell'intera gerarchia di classi che rappresentano
// gli elementi del programma
class RootAST {
public:
  virtual ~RootAST() {};
  virtual void visit() {};
  virtual Value *codegen(driver& drv) { return nullptr; };
};

// Classe che rappresenta la sequenza di statement
class SeqAST : public RootAST {
private:
  RootAST* first;
  RootAST* continuation;

public:
  SeqAST(RootAST* first, RootAST* continuation);
  void visit() override;
  Value *codegen(driver& drv) override;
};

/// ExprAST - Classe base per tutti i nodi espressione
class ExprAST : public RootAST {
protected:
  bool top;
public:
  virtual ~ExprAST() {};
  void toggle();
  bool gettop();
};

/// NumberExprAST - Classe per la rappresentazione di costanti numeriche
class NumberExprAST : public ExprAST {
private:
  double Val;

public:
  NumberExprAST(double Val);
  double GetVal();
  void visit() override;
  Value *codegen(driver& drv) override;
};

/// VariableExprAST - Classe per la rappresentazione di riferimenti a variabili
class VariableExprAST : public ExprAST {
private:
  std::string Name;
  ExprAST* idx = nullptr;  // Utilizzato soltanto per gli array
  
public:
  VariableExprAST(std::string &Name);
  VariableExprAST(std::string &Name, ExprAST* idx);
  const std::string &getName() const;
  void visit() override;
  Value *codegen(driver& drv) override;
};

class UnaryExprAST : public ExprAST {
private:
  char op;
  ExprAST *expr;
  
public:
  UnaryExprAST(char op, ExprAST* expr);
  void visit() override;
  Value *codegen(driver& drv) override;
};

class AssignmentExprAST : public ExprAST {
public:
  std::string id;
private:
  ExprAST *expr;
  ExprAST *idx = nullptr;  // Utilizzato soltanto per gli array
  
public:
  AssignmentExprAST(std::string id, ExprAST* expr, ExprAST* idx);
  void visit() override;
  Value *codegen(driver& drv) override;
};

class VarExprAST : public ExprAST {
private:
  std::vector<Pair> vars;
  ExprAST* body;

  public:
  VarExprAST(std::vector<Pair>& vars, ExprAST* body);
  void visit() override;
  Value* codegen(driver& drv) override;
};

enum BinOp
{
  Op_Add,
  Op_Sub,
  Op_Mul,
  Op_Div,
  Op_LT,
  Op_GT,
  Op_LE,
  Op_GE,
  Op_EQ,
  Op_NE,
  Op_MultiExp,
};

/// BinaryExprAST - Classe per la rappresentazione di operatori binary
class BinaryExprAST : public ExprAST {
private:
  BinOp Op;
  ExprAST* LHS;
  ExprAST* RHS;

public:
  BinaryExprAST(BinOp Op, ExprAST* LHS, ExprAST* RHS);
  void visit() override;
  Value *codegen(driver& drv) override;
};



/// CallExprAST - Classe per la rappresentazione di chiamate di funzione
class CallExprAST : public ExprAST {
private:
  std::string Callee;
  std::vector<ExprAST*> Args;  // ASTs per la valutazione degli argomenti

public:
  CallExprAST(std::string Callee, std::vector<ExprAST*> Args);
  void visit() override;
  Value *codegen(driver& drv) override;
};

/// PrototypeAST - Classe per la rappresentazione dei prototipi di funzione
/// (nome, numero e nome dei parametri; in questo caso il tipo è implicito
/// perché unico)

class PrototypeAST : public RootAST {
private:
  std::string Name;
  std::vector<ProtoArgument> Args;
  bool emit;

public:
  PrototypeAST(std::string Name, std::vector<ProtoArgument> Args);
  const std::string &getName() const;
  const std::vector<ProtoArgument> &getArgs() const; 
  void visit() override;
  Function *codegen(driver& drv) override;
  void noemit();
  bool emitp();
};

/// FunctionAST - Classe che rappresenta la definizione di una funzione
class FunctionAST : public RootAST {
private:
  PrototypeAST* Proto;
  ExprAST* Body;
  bool external;
  
public:
  FunctionAST(PrototypeAST* Proto, ExprAST* Body);
  void visit() override;
  Function *codegen(driver& drv) override;
};

class IfExprAST : public ExprAST {
private:
  ExprAST* cond;
  ExprAST* thenExpr;
  ExprAST* elseExpr;

public:
  IfExprAST(ExprAST* cond, ExprAST* thenExpr, ExprAST* elseExpr);
  void visit() override;
  Value* codegen(driver& drv) override;
};

class ForExprAST : public ExprAST {
private:
  std::string id;
  ExprAST* init;
  ExprAST* cond;
  ExprAST* step;
  ExprAST* body;

public:
  ForExprAST(std::string id, ExprAST* init, ExprAST* cond, ExprAST* step, ExprAST *body);
  void visit() override;
  Value* codegen(driver& drv) override;
};

class WhileExprAST : public ExprAST {
private:
  ExprAST* cond;
  ExprAST* body;

public:
  WhileExprAST(ExprAST* cond, ExprAST* body);
  void visit() override;
  Value* codegen(driver& drv) override;
};

//void InitializeModule();

#endif // ! DRIVER_HH
