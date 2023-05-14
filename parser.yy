%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.2"
%defines

%define api.token.constructor
%define api.location.file none
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  #include <exception>
  class driver;
  class RootAST;
  class ExprAST;
  class FunctionAST;
  class SeqAST;
  class PrototypeAST;
  class IfExprAST;
  class ForExprAST;
  class AssignmentExprAST;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.hh"
}

%define api.token.prefix {TOK_}
%token
  EOF  0  "end of file"
  SEMICOLON  ";"
  MULTIEXP   ":"
  COMMA      ","
  MINUS      "-"
  PLUS       "+"
  STAR       "*"
  SLASH      "/"
  ASSIGN     "="
  LPAREN     "("
  RPAREN     ")"
  LT         "<"
  LE         "<="
  GT         ">"
  GE         ">="
  EQ         "=="
  NEQ        "!="
  EXTERN     "extern"
  DEF        "def"
  IF         "if"
  THEN       "then"
  ELSE       "else"
  END        "end"
  FOR        "for"
  IN         "in"
  VAR        "var"
;

%token <std::string> IDENTIFIER "id"
%token <double> NUMBER "number"
%type <ExprAST*> exp
%type <ExprAST*> unaryexp
%type <ExprAST*> ifexp
%type <ExprAST*> forexp
%type <ExprAST*> step
%type <ExprAST*> idexp
%type <std::vector<ExprAST*>> optexp
%type <std::vector<ExprAST*>> explist
%type <RootAST*> program
%type <RootAST*> top
%type <FunctionAST*> definition
%type <PrototypeAST*> external
%type <PrototypeAST*> proto
%type <std::vector<std::string>> idseq
%type <ExprAST*> assignment
%type <ExprAST*> varexp
%type <std::vector<AssignmentExprAST*>> varlist
%type <AssignmentExprAST*> pair

%%
%start startsymb;

startsymb:
program             { drv.root = $1; }

program:
  %empty               { $$ = new SeqAST(nullptr,nullptr); }
|  top ";" program     { $$ = new SeqAST($1,$3); };

top:
%empty                 { $$ = nullptr; }
| definition           { $$ = $1; }
| external             { $$ = $1; }
| exp                  { $$ = $1; $1->toggle(); };

definition:
  "def" proto exp      { $$ = new FunctionAST($2,$3); $2->noemit(); };

external:
  "extern" proto       { $$ = $2; };

proto:
  "id" "(" idseq ")"   { $$ = new PrototypeAST($1,$3); };

idseq:
  %empty               { std::vector<std::string> args;
                         $$ = args; }
| "id" idseq           { $2.insert($2.begin(),$1); $$ = $2; };

%left ":";
%right "=";
%left "<" ">" "<=" ">=" "==" "!=";
%left "+" "-";
%left "*" "/";

exp:
  exp "+" exp          { $$ = new BinaryExprAST(Op_Add,$1,$3); }
| exp "-" exp          { $$ = new BinaryExprAST(Op_Sub,$1,$3); }
| exp "*" exp          { $$ = new BinaryExprAST(Op_Mul,$1,$3); }
| exp "/" exp          { $$ = new BinaryExprAST(Op_Div,$1,$3); }
| exp "<" exp          { $$ = new BinaryExprAST(Op_LT,$1,$3); }
| exp "<=" exp         { $$ = new BinaryExprAST(Op_LE,$1,$3); }
| exp ">" exp          { $$ = new BinaryExprAST(Op_GT,$1,$3); }
| exp ">=" exp         { $$ = new BinaryExprAST(Op_GE,$1,$3); }
| exp "==" exp         { $$ = new BinaryExprAST(Op_EQ,$1,$3); }
| exp "!=" exp         { $$ = new BinaryExprAST(Op_NE,$1,$3); }
| exp ":" exp          { $$ = new BinaryExprAST(Op_MultiExp,$1,$3); }
| unaryexp             { $$ = $1; }
| idexp                { $$ = $1; }
| "(" exp ")"          { $$ = $2; }
| ifexp                { $$ = $1; }
| forexp               { $$ = $1; }
| assignment           { $$ = $1; }
| varexp               { $$ = $1; }
| "number"             { $$ = new NumberExprAST($1); };

assignment:
  "id" "=" exp         { $$ = new AssignmentExprAST($1,$3); }
;

unaryexp:
  "+" exp              { $$ = new UnaryExprAST('+',$2); }
| "-" exp              { $$ = new UnaryExprAST('-',$2); }
;

varexp:
  "var" varlist "in" exp "end"  { $$ = new VarExprAST($2, $4); }
;

varlist:
  pair                  { std::vector<AssignmentExprAST*> vars;
                          vars.push_back($1);
                          $$ = vars;
                        }
| pair "," varlist      {
                          $3.insert($3.begin(), $1);
                          $$ = $3;
                        }
;

pair:
  "id"                  { $$ = new AssignmentExprAST($1, nullptr); }
| "id" "=" exp          { $$ = new AssignmentExprAST($1, $3); }
;

idexp:
  "id"                 { $$ = new VariableExprAST($1); }
| "id" "(" optexp ")"  { $$ = new CallExprAST($1,$3); };

optexp:
%empty                 { std::vector<ExprAST*> args;
                         args.push_back(nullptr);
			 $$ = args;
                       }
| explist              { $$ = $1; };

explist:
  exp                  { std::vector<ExprAST*> args;
                         args.push_back($1);
			 $$ = args;
                       }
| exp "," explist      { $3.insert($3.begin(), $1); $$ = $3; };

ifexp:
  "if" exp "then" exp "else" exp "end" { $$ = new IfExprAST($2, $4, $6); }
;

forexp:
  "for" "id" "=" exp "," exp step "in" exp "end"  { $$ = new ForExprAST($2, $4, $6, $7, $9); }
;

step:
  %empty               { $$ = nullptr; }
| "," exp                  { $$ = $2; }
;

%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
