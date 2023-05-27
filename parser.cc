// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hh"


// Unqualified %code blocks.
#line 52 "parser.yy"

# include "driver.hh"

#line 50 "parser.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 142 "parser.cc"

  /// Build a parser object.
  parser::parser (driver& drv_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_exp: // exp
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_unaryexp: // unaryexp
      case symbol_kind::S_varexp: // varexp
      case symbol_kind::S_optarray: // optarray
      case symbol_kind::S_idexp: // idexp
      case symbol_kind::S_ifexp: // ifexp
      case symbol_kind::S_forexp: // forexp
      case symbol_kind::S_step: // step
      case symbol_kind::S_whileexp: // whileexp
        value.YY_MOVE_OR_COPY< ExprAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
        value.YY_MOVE_OR_COPY< FunctionAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pair: // pair
        value.YY_MOVE_OR_COPY< Pair > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< ProtoArgument > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.YY_MOVE_OR_COPY< PrototypeAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
        value.YY_MOVE_OR_COPY< RootAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.YY_MOVE_OR_COPY< std::vector<ExprAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_varlist: // varlist
        value.YY_MOVE_OR_COPY< std::vector<Pair> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_idseq: // idseq
        value.YY_MOVE_OR_COPY< std::vector<ProtoArgument> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_exp: // exp
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_unaryexp: // unaryexp
      case symbol_kind::S_varexp: // varexp
      case symbol_kind::S_optarray: // optarray
      case symbol_kind::S_idexp: // idexp
      case symbol_kind::S_ifexp: // ifexp
      case symbol_kind::S_forexp: // forexp
      case symbol_kind::S_step: // step
      case symbol_kind::S_whileexp: // whileexp
        value.move< ExprAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_definition: // definition
        value.move< FunctionAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_pair: // pair
        value.move< Pair > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_argument: // argument
        value.move< ProtoArgument > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.move< PrototypeAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
        value.move< RootAST* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< double > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.move< std::vector<ExprAST*> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_varlist: // varlist
        value.move< std::vector<Pair> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_idseq: // idseq
        value.move< std::vector<ProtoArgument> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_exp: // exp
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_unaryexp: // unaryexp
      case symbol_kind::S_varexp: // varexp
      case symbol_kind::S_optarray: // optarray
      case symbol_kind::S_idexp: // idexp
      case symbol_kind::S_ifexp: // ifexp
      case symbol_kind::S_forexp: // forexp
      case symbol_kind::S_step: // step
      case symbol_kind::S_whileexp: // whileexp
        value.copy< ExprAST* > (that.value);
        break;

      case symbol_kind::S_definition: // definition
        value.copy< FunctionAST* > (that.value);
        break;

      case symbol_kind::S_pair: // pair
        value.copy< Pair > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.copy< ProtoArgument > (that.value);
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.copy< PrototypeAST* > (that.value);
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
        value.copy< RootAST* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.copy< double > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.copy< std::vector<ExprAST*> > (that.value);
        break;

      case symbol_kind::S_varlist: // varlist
        value.copy< std::vector<Pair> > (that.value);
        break;

      case symbol_kind::S_idseq: // idseq
        value.copy< std::vector<ProtoArgument> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_exp: // exp
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_unaryexp: // unaryexp
      case symbol_kind::S_varexp: // varexp
      case symbol_kind::S_optarray: // optarray
      case symbol_kind::S_idexp: // idexp
      case symbol_kind::S_ifexp: // ifexp
      case symbol_kind::S_forexp: // forexp
      case symbol_kind::S_step: // step
      case symbol_kind::S_whileexp: // whileexp
        value.move< ExprAST* > (that.value);
        break;

      case symbol_kind::S_definition: // definition
        value.move< FunctionAST* > (that.value);
        break;

      case symbol_kind::S_pair: // pair
        value.move< Pair > (that.value);
        break;

      case symbol_kind::S_argument: // argument
        value.move< ProtoArgument > (that.value);
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        value.move< PrototypeAST* > (that.value);
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
        value.move< RootAST* > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "number"
        value.move< double > (that.value);
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        value.move< std::vector<ExprAST*> > (that.value);
        break;

      case symbol_kind::S_varlist: // varlist
        value.move< std::vector<Pair> > (that.value);
        break;

      case symbol_kind::S_idseq: // idseq
        value.move< std::vector<ProtoArgument> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (drv));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_exp: // exp
      case symbol_kind::S_assignment: // assignment
      case symbol_kind::S_unaryexp: // unaryexp
      case symbol_kind::S_varexp: // varexp
      case symbol_kind::S_optarray: // optarray
      case symbol_kind::S_idexp: // idexp
      case symbol_kind::S_ifexp: // ifexp
      case symbol_kind::S_forexp: // forexp
      case symbol_kind::S_step: // step
      case symbol_kind::S_whileexp: // whileexp
        yylhs.value.emplace< ExprAST* > ();
        break;

      case symbol_kind::S_definition: // definition
        yylhs.value.emplace< FunctionAST* > ();
        break;

      case symbol_kind::S_pair: // pair
        yylhs.value.emplace< Pair > ();
        break;

      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< ProtoArgument > ();
        break;

      case symbol_kind::S_external: // external
      case symbol_kind::S_proto: // proto
        yylhs.value.emplace< PrototypeAST* > ();
        break;

      case symbol_kind::S_program: // program
      case symbol_kind::S_top: // top
        yylhs.value.emplace< RootAST* > ();
        break;

      case symbol_kind::S_NUMBER: // "number"
        yylhs.value.emplace< double > ();
        break;

      case symbol_kind::S_IDENTIFIER: // "id"
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_optexp: // optexp
      case symbol_kind::S_explist: // explist
        yylhs.value.emplace< std::vector<ExprAST*> > ();
        break;

      case symbol_kind::S_varlist: // varlist
        yylhs.value.emplace< std::vector<Pair> > ();
        break;

      case symbol_kind::S_idseq: // idseq
        yylhs.value.emplace< std::vector<ProtoArgument> > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // startsymb: program
#line 119 "parser.yy"
                    { drv.root = yystack_[0].value.as < RootAST* > (); }
#line 816 "parser.cc"
    break;

  case 3: // program: %empty
#line 122 "parser.yy"
                       { yylhs.value.as < RootAST* > () = new SeqAST(nullptr,nullptr); }
#line 822 "parser.cc"
    break;

  case 4: // program: top ";" program
#line 123 "parser.yy"
                       { yylhs.value.as < RootAST* > () = new SeqAST(yystack_[2].value.as < RootAST* > (),yystack_[0].value.as < RootAST* > ()); }
#line 828 "parser.cc"
    break;

  case 5: // top: %empty
#line 126 "parser.yy"
                       { yylhs.value.as < RootAST* > () = nullptr; }
#line 834 "parser.cc"
    break;

  case 6: // top: definition
#line 127 "parser.yy"
                       { yylhs.value.as < RootAST* > () = yystack_[0].value.as < FunctionAST* > (); }
#line 840 "parser.cc"
    break;

  case 7: // top: external
#line 128 "parser.yy"
                       { yylhs.value.as < RootAST* > () = yystack_[0].value.as < PrototypeAST* > (); }
#line 846 "parser.cc"
    break;

  case 8: // top: exp
#line 129 "parser.yy"
                       { yylhs.value.as < RootAST* > () = yystack_[0].value.as < ExprAST* > (); yystack_[0].value.as < ExprAST* > ()->toggle(); }
#line 852 "parser.cc"
    break;

  case 9: // definition: "def" proto exp
#line 132 "parser.yy"
                       { yylhs.value.as < FunctionAST* > () = new FunctionAST(yystack_[1].value.as < PrototypeAST* > (),yystack_[0].value.as < ExprAST* > ()); yystack_[1].value.as < PrototypeAST* > ()->noemit(); }
#line 858 "parser.cc"
    break;

  case 10: // external: "extern" proto
#line 135 "parser.yy"
                       { yylhs.value.as < PrototypeAST* > () = yystack_[0].value.as < PrototypeAST* > (); }
#line 864 "parser.cc"
    break;

  case 11: // proto: "id" "(" idseq ")"
#line 138 "parser.yy"
                       { yylhs.value.as < PrototypeAST* > () = new PrototypeAST(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::vector<ProtoArgument> > ()); }
#line 870 "parser.cc"
    break;

  case 12: // idseq: %empty
#line 141 "parser.yy"
                       { std::vector<ProtoArgument> args;
                         yylhs.value.as < std::vector<ProtoArgument> > () = args; }
#line 877 "parser.cc"
    break;

  case 13: // idseq: argument idseq
#line 143 "parser.yy"
                       { yystack_[0].value.as < std::vector<ProtoArgument> > ().insert(yystack_[0].value.as < std::vector<ProtoArgument> > ().begin(),yystack_[1].value.as < ProtoArgument > ()); yylhs.value.as < std::vector<ProtoArgument> > () = yystack_[0].value.as < std::vector<ProtoArgument> > (); }
#line 883 "parser.cc"
    break;

  case 14: // argument: "id"
#line 147 "parser.yy"
                      { ProtoArgument arg;
                        arg.name = yystack_[0].value.as < std::string > ();
                        arg.isPointer = false;
                        yylhs.value.as < ProtoArgument > () = arg;
                      }
#line 893 "parser.cc"
    break;

  case 15: // argument: "id" "[" "]"
#line 152 "parser.yy"
                      { ProtoArgument arg;
                        arg.name = yystack_[2].value.as < std::string > ();
                        arg.isPointer = true;
                        yylhs.value.as < ProtoArgument > () = arg;
                      }
#line 903 "parser.cc"
    break;

  case 16: // exp: exp "+" exp
#line 165 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_Add,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 909 "parser.cc"
    break;

  case 17: // exp: exp "-" exp
#line 166 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_Sub,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 915 "parser.cc"
    break;

  case 18: // exp: exp "*" exp
#line 167 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_Mul,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 921 "parser.cc"
    break;

  case 19: // exp: exp "/" exp
#line 168 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_Div,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 927 "parser.cc"
    break;

  case 20: // exp: exp "<" exp
#line 169 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_LT,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 933 "parser.cc"
    break;

  case 21: // exp: exp "<=" exp
#line 170 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_LE,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 939 "parser.cc"
    break;

  case 22: // exp: exp ">" exp
#line 171 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_GT,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 945 "parser.cc"
    break;

  case 23: // exp: exp ">=" exp
#line 172 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_GE,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 951 "parser.cc"
    break;

  case 24: // exp: exp "==" exp
#line 173 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_EQ,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 957 "parser.cc"
    break;

  case 25: // exp: exp "!=" exp
#line 174 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_NE,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 963 "parser.cc"
    break;

  case 26: // exp: exp ":" exp
#line 175 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new BinaryExprAST(Op_MultiExp,yystack_[2].value.as < ExprAST* > (),yystack_[0].value.as < ExprAST* > ()); }
#line 969 "parser.cc"
    break;

  case 27: // exp: unaryexp
#line 176 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 975 "parser.cc"
    break;

  case 28: // exp: idexp
#line 177 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 981 "parser.cc"
    break;

  case 29: // exp: "(" exp ")"
#line 178 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[1].value.as < ExprAST* > (); }
#line 987 "parser.cc"
    break;

  case 30: // exp: ifexp
#line 179 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 993 "parser.cc"
    break;

  case 31: // exp: forexp
#line 180 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 999 "parser.cc"
    break;

  case 32: // exp: assignment
#line 181 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1005 "parser.cc"
    break;

  case 33: // exp: varexp
#line 182 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1011 "parser.cc"
    break;

  case 34: // exp: whileexp
#line 183 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1017 "parser.cc"
    break;

  case 35: // exp: "number"
#line 184 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new NumberExprAST(yystack_[0].value.as < double > ()); }
#line 1023 "parser.cc"
    break;

  case 36: // assignment: "id" optarray "=" exp
#line 188 "parser.yy"
                                { yylhs.value.as < ExprAST* > () = new AssignmentExprAST(yystack_[3].value.as < std::string > (),yystack_[0].value.as < ExprAST* > (),yystack_[2].value.as < ExprAST* > ()); }
#line 1029 "parser.cc"
    break;

  case 37: // unaryexp: "+" exp
#line 191 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new UnaryExprAST('+',yystack_[0].value.as < ExprAST* > ()); }
#line 1035 "parser.cc"
    break;

  case 38: // unaryexp: "-" exp
#line 192 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new UnaryExprAST('-',yystack_[0].value.as < ExprAST* > ()); }
#line 1041 "parser.cc"
    break;

  case 39: // varexp: "var" varlist "in" exp "end"
#line 196 "parser.yy"
                                { yylhs.value.as < ExprAST* > () = new VarExprAST(yystack_[3].value.as < std::vector<Pair> > (), yystack_[1].value.as < ExprAST* > ()); }
#line 1047 "parser.cc"
    break;

  case 40: // varlist: pair
#line 200 "parser.yy"
                        { std::vector<Pair> vars;
                          vars.push_back(yystack_[0].value.as < Pair > ());
                          yylhs.value.as < std::vector<Pair> > () = vars;
                        }
#line 1056 "parser.cc"
    break;

  case 41: // varlist: pair "," varlist
#line 204 "parser.yy"
                        {
                          yystack_[0].value.as < std::vector<Pair> > ().insert(yystack_[0].value.as < std::vector<Pair> > ().begin(), yystack_[2].value.as < Pair > ());
                          yylhs.value.as < std::vector<Pair> > () = yystack_[0].value.as < std::vector<Pair> > ();
                        }
#line 1065 "parser.cc"
    break;

  case 42: // pair: "id" optarray
#line 211 "parser.yy"
                                    { yylhs.value.as < Pair > () = { yystack_[1].value.as < std::string > (), nullptr, yystack_[0].value.as < ExprAST* > () }; }
#line 1071 "parser.cc"
    break;

  case 43: // pair: "id" optarray "=" exp
#line 212 "parser.yy"
                                    { yylhs.value.as < Pair > () = { yystack_[3].value.as < std::string > (), yystack_[0].value.as < ExprAST* > (), yystack_[2].value.as < ExprAST* > () }; }
#line 1077 "parser.cc"
    break;

  case 44: // pair: "id" optarray "=" "{" explist "}"
#line 213 "parser.yy"
                                    { yylhs.value.as < Pair > () = { yystack_[5].value.as < std::string > (), nullptr, yystack_[4].value.as < ExprAST* > (), yystack_[1].value.as < std::vector<ExprAST*> > () }; }
#line 1083 "parser.cc"
    break;

  case 45: // optarray: %empty
#line 217 "parser.yy"
               { yylhs.value.as < ExprAST* > () = nullptr; }
#line 1089 "parser.cc"
    break;

  case 46: // optarray: "[" exp "]"
#line 218 "parser.yy"
               { yylhs.value.as < ExprAST* > () = yystack_[1].value.as < ExprAST* > (); }
#line 1095 "parser.cc"
    break;

  case 47: // idexp: "id"
#line 222 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new VariableExprAST(yystack_[0].value.as < std::string > ()); }
#line 1101 "parser.cc"
    break;

  case 48: // idexp: "id" "[" exp "]"
#line 223 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new VariableExprAST(yystack_[3].value.as < std::string > (), yystack_[1].value.as < ExprAST* > ()); }
#line 1107 "parser.cc"
    break;

  case 49: // idexp: "id" "(" optexp ")"
#line 224 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = new CallExprAST(yystack_[3].value.as < std::string > (),yystack_[1].value.as < std::vector<ExprAST*> > ()); }
#line 1113 "parser.cc"
    break;

  case 50: // optexp: %empty
#line 227 "parser.yy"
                       { std::vector<ExprAST*> args;
                         args.push_back(nullptr);
			 yylhs.value.as < std::vector<ExprAST*> > () = args;
                       }
#line 1122 "parser.cc"
    break;

  case 51: // optexp: explist
#line 231 "parser.yy"
                       { yylhs.value.as < std::vector<ExprAST*> > () = yystack_[0].value.as < std::vector<ExprAST*> > (); }
#line 1128 "parser.cc"
    break;

  case 52: // explist: exp
#line 234 "parser.yy"
                       { std::vector<ExprAST*> args;
                         args.push_back(yystack_[0].value.as < ExprAST* > ());
			                   yylhs.value.as < std::vector<ExprAST*> > () = args;
                       }
#line 1137 "parser.cc"
    break;

  case 53: // explist: exp "," explist
#line 238 "parser.yy"
                       { yystack_[0].value.as < std::vector<ExprAST*> > ().insert(yystack_[0].value.as < std::vector<ExprAST*> > ().begin(), yystack_[2].value.as < ExprAST* > ()); yylhs.value.as < std::vector<ExprAST*> > () = yystack_[0].value.as < std::vector<ExprAST*> > (); }
#line 1143 "parser.cc"
    break;

  case 54: // ifexp: "if" exp "then" exp "else" exp "end"
#line 241 "parser.yy"
                                       { yylhs.value.as < ExprAST* > () = new IfExprAST(yystack_[5].value.as < ExprAST* > (), yystack_[3].value.as < ExprAST* > (), yystack_[1].value.as < ExprAST* > ()); }
#line 1149 "parser.cc"
    break;

  case 55: // forexp: "for" "id" "=" exp "," exp step "in" exp "end"
#line 245 "parser.yy"
                                                  { yylhs.value.as < ExprAST* > () = new ForExprAST(yystack_[8].value.as < std::string > (), yystack_[6].value.as < ExprAST* > (), yystack_[4].value.as < ExprAST* > (), yystack_[3].value.as < ExprAST* > (), yystack_[1].value.as < ExprAST* > ()); }
#line 1155 "parser.cc"
    break;

  case 56: // step: %empty
#line 249 "parser.yy"
                       { yylhs.value.as < ExprAST* > () = nullptr; }
#line 1161 "parser.cc"
    break;

  case 57: // step: "," exp
#line 250 "parser.yy"
                           { yylhs.value.as < ExprAST* > () = yystack_[0].value.as < ExprAST* > (); }
#line 1167 "parser.cc"
    break;

  case 58: // whileexp: "while" exp "in" exp "end"
#line 254 "parser.yy"
                                  { yylhs.value.as < ExprAST* > () = new WhileExprAST(yystack_[3].value.as < ExprAST* > (), yystack_[1].value.as < ExprAST* > ()); }
#line 1173 "parser.cc"
    break;


#line 1177 "parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yyerror_range[1].location = yystack_[0].location;
        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -87;

  const signed char parser::yytable_ninf_ = -47;

  const short
  parser::yypact_[] =
  {
      58,    98,    98,    98,   -28,   -28,    98,   -26,   -14,    98,
       5,   -87,    27,   -87,    25,   -87,   -87,   341,   -87,   -87,
     -87,   -87,   -87,   -87,   -87,     9,     9,   264,    18,   -87,
      98,   243,    21,    17,     3,    30,     4,    98,    98,    26,
     -87,    58,    98,    98,    98,    98,    98,    98,    98,    98,
      98,    98,    98,   -87,     6,   341,    98,    98,    98,    43,
      98,   -14,    98,   283,    28,   -87,   290,    98,   -87,   146,
       9,     9,   -87,   -87,    64,    64,    64,    64,    64,    64,
      22,    42,     6,   221,   309,   316,    88,   129,   -87,   152,
      98,   -87,    45,   146,    47,   -87,   -87,    98,    98,   -87,
      98,   341,   -87,   -87,   -87,   -87,   175,   335,    52,   -87,
      98,    29,   -87,   341,    98,   198,   -87
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      47,    35,     0,     2,     0,     6,     7,     8,    32,    27,
      33,    28,    30,    31,    34,    38,    37,     0,     0,    10,
       0,     0,     0,    45,     0,    40,     0,    50,     0,     0,
       1,     3,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    29,    12,     9,     0,     0,     0,    42,
       0,     0,     0,    52,     0,    51,     0,     0,     4,    26,
      17,    16,    18,    19,    20,    21,    22,    23,    24,    25,
      14,     0,    12,     0,     0,     0,     0,     0,    41,     0,
       0,    49,    48,    36,     0,    11,    13,     0,     0,    46,
       0,    43,    39,    58,    53,    15,     0,    56,     0,    54,
       0,     0,    44,    57,     0,     0,    55
  };

  const signed char
  parser::yypgoto_[] =
  {
     -87,   -87,    33,   -87,   -87,   -87,    63,    -7,   -87,     0,
     -87,   -87,   -87,    15,   -87,    44,   -87,   -87,   -86,   -87,
     -87,   -87,   -87
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,    12,    13,    14,    15,    16,    29,    81,    82,    63,
      18,    19,    20,    34,    35,    39,    21,    64,    65,    22,
      23,   111,    24
  };

  const signed char
  parser::yytable_[] =
  {
      17,    25,    26,    27,   104,    28,    31,    32,    42,    36,
      43,    44,    45,    46,   108,   -45,    37,    45,    46,    33,
      38,    47,    48,    49,    50,    51,    52,    40,    41,    54,
      55,    57,    58,    60,    62,    61,    67,    94,    66,    80,
      91,    17,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    86,    95,   -46,    83,    84,    85,   114,
      87,    -5,    89,   105,     1,     2,   112,    93,    30,     3,
      43,    44,    45,    46,    68,    96,    88,    59,     0,     0,
       0,     4,     5,     6,     0,     0,   101,     7,     0,     8,
       9,    10,    11,     0,     1,     2,     0,   106,   107,     3,
       0,   100,     0,     0,     1,     2,     0,     0,     0,     3,
     113,     0,     0,     6,   115,     0,     0,     7,     0,     8,
       9,    10,    11,     6,     0,     0,     0,     7,     0,     8,
       9,    10,    11,    42,     0,    43,    44,    45,    46,     0,
       0,     0,     0,     0,     0,     0,    47,    48,    49,    50,
      51,    52,    43,    44,    45,    46,    42,   102,    43,    44,
      45,    46,     0,    47,    48,    49,    50,    51,    52,    47,
      48,    49,    50,    51,    52,     0,     0,     0,     0,    42,
     103,    43,    44,    45,    46,     0,     0,     0,     0,     0,
       0,     0,    47,    48,    49,    50,    51,    52,     0,     0,
       0,     0,    42,   109,    43,    44,    45,    46,     0,     0,
       0,     0,     0,     0,     0,    47,    48,    49,    50,    51,
      52,     0,     0,     0,     0,    42,   116,    43,    44,    45,
      46,     0,     0,     0,     0,     0,     0,     0,    47,    48,
      49,    50,    51,    52,     0,     0,     0,    42,    97,    43,
      44,    45,    46,     0,     0,     0,     0,     0,     0,     0,
      47,    48,    49,    50,    51,    52,     0,     0,    42,    56,
      43,    44,    45,    46,     0,     0,    53,     0,     0,     0,
       0,    47,    48,    49,    50,    51,    52,    42,    90,    43,
      44,    45,    46,     0,    42,     0,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    92,    47,    48,    49,
      50,    51,    52,    42,    98,    43,    44,    45,    46,     0,
      42,     0,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    99,    47,    48,    49,    50,    51,    52,    42,
     110,    43,    44,    45,    46,    42,     0,    43,    44,    45,
      46,     0,    47,    48,    49,    50,    51,    52,    47,    48,
      49,    50,    51,    52
  };

  const signed char
  parser::yycheck_[] =
  {
       0,     1,     2,     3,    90,    33,     6,    33,     4,     9,
       6,     7,     8,     9,   100,    10,    11,     8,     9,    33,
      15,    17,    18,    19,    20,    21,    22,     0,     3,    11,
      30,    10,    15,    30,    30,     5,    10,    15,    38,    33,
      12,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    10,    12,    10,    56,    57,    58,    30,
      60,     3,    62,    16,     6,     7,    14,    67,     5,    11,
       6,     7,     8,     9,    41,    82,    61,    33,    -1,    -1,
      -1,    23,    24,    25,    -1,    -1,    86,    29,    -1,    31,
      32,    33,    34,    -1,     6,     7,    -1,    97,    98,    11,
      -1,    13,    -1,    -1,     6,     7,    -1,    -1,    -1,    11,
     110,    -1,    -1,    25,   114,    -1,    -1,    29,    -1,    31,
      32,    33,    34,    25,    -1,    -1,    -1,    29,    -1,    31,
      32,    33,    34,     4,    -1,     6,     7,     8,     9,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    17,    18,    19,    20,
      21,    22,     6,     7,     8,     9,     4,    28,     6,     7,
       8,     9,    -1,    17,    18,    19,    20,    21,    22,    17,
      18,    19,    20,    21,    22,    -1,    -1,    -1,    -1,     4,
      28,     6,     7,     8,     9,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    17,    18,    19,    20,    21,    22,    -1,    -1,
      -1,    -1,     4,    28,     6,     7,     8,     9,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    17,    18,    19,    20,    21,
      22,    -1,    -1,    -1,    -1,     4,    28,     6,     7,     8,
       9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    17,    18,
      19,    20,    21,    22,    -1,    -1,    -1,     4,    27,     6,
       7,     8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      17,    18,    19,    20,    21,    22,    -1,    -1,     4,    26,
       6,     7,     8,     9,    -1,    -1,    12,    -1,    -1,    -1,
      -1,    17,    18,    19,    20,    21,    22,     4,     5,     6,
       7,     8,     9,    -1,     4,    -1,     6,     7,     8,     9,
      17,    18,    19,    20,    21,    22,    16,    17,    18,    19,
      20,    21,    22,     4,     5,     6,     7,     8,     9,    -1,
       4,    -1,     6,     7,     8,     9,    17,    18,    19,    20,
      21,    22,    16,    17,    18,    19,    20,    21,    22,     4,
       5,     6,     7,     8,     9,     4,    -1,     6,     7,     8,
       9,    -1,    17,    18,    19,    20,    21,    22,    17,    18,
      19,    20,    21,    22
  };

  const signed char
  parser::yystos_[] =
  {
       0,     6,     7,    11,    23,    24,    25,    29,    31,    32,
      33,    34,    36,    37,    38,    39,    40,    44,    45,    46,
      47,    51,    54,    55,    57,    44,    44,    44,    33,    41,
      41,    44,    33,    33,    48,    49,    44,    11,    15,    50,
       0,     3,     4,     6,     7,     8,     9,    17,    18,    19,
      20,    21,    22,    12,    11,    44,    26,    10,    15,    50,
      30,     5,    30,    44,    52,    53,    44,    10,    37,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      33,    42,    43,    44,    44,    44,    10,    44,    48,    44,
       5,    12,    16,    44,    15,    12,    42,    27,     5,    16,
      13,    44,    28,    28,    53,    16,    44,    44,    53,    28,
       5,    56,    14,    44,    30,    44,    28
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    35,    36,    37,    37,    38,    38,    38,    38,    39,
      40,    41,    42,    42,    43,    43,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    45,    46,    46,    47,
      48,    48,    49,    49,    49,    50,    50,    51,    51,    51,
      52,    52,    53,    53,    54,    55,    56,    56,    57
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     0,     3,     0,     1,     1,     1,     3,
       2,     4,     0,     2,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     3,
       1,     1,     1,     1,     1,     1,     4,     2,     2,     5,
       1,     3,     2,     4,     6,     0,     3,     1,     4,     4,
       0,     1,     1,     3,     7,    10,     0,     2,     5
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\";\"", "\":\"",
  "\",\"", "\"-\"", "\"+\"", "\"*\"", "\"/\"", "\"=\"", "\"(\"", "\")\"",
  "\"{\"", "\"}\"", "\"[\"", "\"]\"", "\"<\"", "\"<=\"", "\">\"", "\">=\"",
  "\"==\"", "\"!=\"", "\"extern\"", "\"def\"", "\"if\"", "\"then\"",
  "\"else\"", "\"end\"", "\"for\"", "\"in\"", "\"var\"", "\"while\"",
  "\"id\"", "\"number\"", "$accept", "startsymb", "program", "top",
  "definition", "external", "proto", "idseq", "argument", "exp",
  "assignment", "unaryexp", "varexp", "varlist", "pair", "optarray",
  "idexp", "optexp", "explist", "ifexp", "forexp", "step", "whileexp", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,   119,   119,   122,   123,   126,   127,   128,   129,   132,
     135,   138,   141,   143,   147,   152,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   188,   191,   192,   196,
     200,   204,   211,   212,   213,   217,   218,   222,   223,   224,
     227,   231,   234,   238,   241,   245,   249,   250,   254
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 1767 "parser.cc"

#line 256 "parser.yy"


void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}
