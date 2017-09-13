// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

// Take the name prefix into account.
#define yylex   parser4_lex

// First part of user declarations.

#line 39 "dhcp4_parser.cc" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "dhcp4_parser.h"

// User implementation prologue.

#line 53 "dhcp4_parser.cc" // lalr1.cc:412
// Unqualified %code blocks.
#line 34 "dhcp4_parser.yy" // lalr1.cc:413

#include <dhcp4/parser_context.h>

#line 59 "dhcp4_parser.cc" // lalr1.cc:413


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
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if PARSER4_DEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
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
      yystack_print_ ();                \
  } while (false)

#else // !PARSER4_DEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !PARSER4_DEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 14 "dhcp4_parser.yy" // lalr1.cc:479
namespace isc { namespace dhcp {
#line 145 "dhcp4_parser.cc" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Dhcp4Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
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
              // Fall through.
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


  /// Build a parser object.
  Dhcp4Parser::Dhcp4Parser (isc::dhcp::Parser4Context& ctx_yyarg)
    :
#if PARSER4_DEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      ctx (ctx_yyarg)
  {}

  Dhcp4Parser::~Dhcp4Parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  Dhcp4Parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  Dhcp4Parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  Dhcp4Parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  Dhcp4Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  Dhcp4Parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  Dhcp4Parser::symbol_number_type
  Dhcp4Parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  Dhcp4Parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  Dhcp4Parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 159: // value
      case 163: // map_value
      case 201: // socket_type
      case 211: // db_type
      case 288: // hr_mode
      case 429: // ncr_protocol_value
      case 437: // replace_client_name_value
        value.move< ElementPtr > (that.value);
        break;

      case 145: // "boolean"
        value.move< bool > (that.value);
        break;

      case 144: // "floating point"
        value.move< double > (that.value);
        break;

      case 143: // "integer"
        value.move< int64_t > (that.value);
        break;

      case 142: // "constant string"
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  Dhcp4Parser::stack_symbol_type&
  Dhcp4Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 159: // value
      case 163: // map_value
      case 201: // socket_type
      case 211: // db_type
      case 288: // hr_mode
      case 429: // ncr_protocol_value
      case 437: // replace_client_name_value
        value.copy< ElementPtr > (that.value);
        break;

      case 145: // "boolean"
        value.copy< bool > (that.value);
        break;

      case 144: // "floating point"
        value.copy< double > (that.value);
        break;

      case 143: // "integer"
        value.copy< int64_t > (that.value);
        break;

      case 142: // "constant string"
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  Dhcp4Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if PARSER4_DEBUG
  template <typename Base>
  void
  Dhcp4Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 142: // "constant string"

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< std::string > (); }
#line 364 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 143: // "integer"

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< int64_t > (); }
#line 371 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 144: // "floating point"

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< double > (); }
#line 378 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 145: // "boolean"

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< bool > (); }
#line 385 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 159: // value

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 392 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 163: // map_value

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 399 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 201: // socket_type

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 406 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 211: // db_type

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 413 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 288: // hr_mode

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 420 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 429: // ncr_protocol_value

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 427 "dhcp4_parser.cc" // lalr1.cc:636
        break;

      case 437: // replace_client_name_value

#line 219 "dhcp4_parser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ElementPtr > (); }
#line 434 "dhcp4_parser.cc" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  Dhcp4Parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  Dhcp4Parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  Dhcp4Parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if PARSER4_DEBUG
  std::ostream&
  Dhcp4Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Dhcp4Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Dhcp4Parser::debug_level_type
  Dhcp4Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Dhcp4Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // PARSER4_DEBUG

  inline Dhcp4Parser::state_type
  Dhcp4Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  Dhcp4Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Dhcp4Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Dhcp4Parser::parse ()
  {
    // State.
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

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (ctx));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 159: // value
      case 163: // map_value
      case 201: // socket_type
      case 211: // db_type
      case 288: // hr_mode
      case 429: // ncr_protocol_value
      case 437: // replace_client_name_value
        yylhs.value.build< ElementPtr > ();
        break;

      case 145: // "boolean"
        yylhs.value.build< bool > ();
        break;

      case 144: // "floating point"
        yylhs.value.build< double > ();
        break;

      case 143: // "integer"
        yylhs.value.build< int64_t > ();
        break;

      case 142: // "constant string"
        yylhs.value.build< std::string > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 228 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.NO_KEYWORD; }
#line 680 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 4:
#line 229 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.CONFIG; }
#line 686 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 6:
#line 230 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.DHCP4; }
#line 692 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 8:
#line 231 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.INTERFACES_CONFIG; }
#line 698 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 10:
#line 232 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.SUBNET4; }
#line 704 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 12:
#line 233 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.POOLS; }
#line 710 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 14:
#line 234 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.RESERVATIONS; }
#line 716 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 16:
#line 235 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.OPTION_DEF; }
#line 722 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 18:
#line 236 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.OPTION_DATA; }
#line 728 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 20:
#line 237 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.HOOKS_LIBRARIES; }
#line 734 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 22:
#line 238 "dhcp4_parser.yy" // lalr1.cc:859
    { ctx.ctx_ = ctx.DHCP_DDNS; }
#line 740 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 24:
#line 246 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location))); }
#line 746 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 25:
#line 247 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new DoubleElement(yystack_[0].value.as< double > (), ctx.loc2pos(yystack_[0].location))); }
#line 752 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 26:
#line 248 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location))); }
#line 758 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 27:
#line 249 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location))); }
#line 764 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 28:
#line 250 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new NullElement(ctx.loc2pos(yystack_[0].location))); }
#line 770 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 29:
#line 251 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 776 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 30:
#line 252 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 782 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 31:
#line 255 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Push back the JSON value on the stack
    ctx.stack_.push_back(yystack_[0].value.as< ElementPtr > ());
}
#line 791 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 32:
#line 260 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 802 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 33:
#line 265 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
}
#line 812 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 34:
#line 271 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ctx.stack_.back(); ctx.stack_.pop_back(); }
#line 818 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 37:
#line 278 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // map containing a single entry
                  ctx.stack_.back()->set(yystack_[2].value.as< std::string > (), yystack_[0].value.as< ElementPtr > ());
                  }
#line 827 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 38:
#line 282 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // map consisting of a shorter map followed by
                  // comma and string:value
                  ctx.stack_.back()->set(yystack_[2].value.as< std::string > (), yystack_[0].value.as< ElementPtr > ());
                  }
#line 837 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 39:
#line 289 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(l);
}
#line 846 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 40:
#line 292 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // list parsing complete. Put any sanity checking here
}
#line 854 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 43:
#line 300 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // List consisting of a single element.
                  ctx.stack_.back()->add(yystack_[0].value.as< ElementPtr > ());
                  }
#line 863 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 44:
#line 304 "dhcp4_parser.yy" // lalr1.cc:859
    {
                  // List ending with , and a value.
                  ctx.stack_.back()->add(yystack_[0].value.as< ElementPtr > ());
                  }
#line 872 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 45:
#line 311 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // List parsing about to start
}
#line 880 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 46:
#line 313 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // list parsing complete. Put any sanity checking here
    //ctx.stack_.pop_back();
}
#line 889 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 49:
#line 322 "dhcp4_parser.yy" // lalr1.cc:859
    {
                          ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
                          ctx.stack_.back()->add(s);
                          }
#line 898 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 50:
#line 326 "dhcp4_parser.yy" // lalr1.cc:859
    {
                          ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
                          ctx.stack_.back()->add(s);
                          }
#line 907 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 51:
#line 337 "dhcp4_parser.yy" // lalr1.cc:859
    {
    const std::string& where = ctx.contextName();
    const std::string& keyword = yystack_[1].value.as< std::string > ();
    error(yystack_[1].location,
          "got unexpected keyword \"" + keyword + "\" in " + where + " map.");
}
#line 918 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 52:
#line 347 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 929 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 53:
#line 352 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
}
#line 939 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 62:
#line 373 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // This code is executed when we're about to start parsing
    // the content of the map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("Dhcp4", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.DHCP4);
}
#line 952 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 63:
#line 380 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // map parsing completed. If we ever want to do any wrap up
    // (maybe some sanity checking), this would be the best place
    // for it.
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 964 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 64:
#line 390 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the Dhcp4 map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 974 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 65:
#line 394 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 982 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 90:
#line 428 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("valid-lifetime", prf);
}
#line 991 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 91:
#line 433 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("renew-timer", prf);
}
#line 1000 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 92:
#line 438 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("rebind-timer", prf);
}
#line 1009 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 93:
#line 443 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr dpp(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("decline-probation-period", dpp);
}
#line 1018 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 94:
#line 448 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr echo(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("echo-client-id", echo);
}
#line 1027 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 95:
#line 453 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr match(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("match-client-id", match);
}
#line 1036 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 96:
#line 459 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interfaces-config", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.INTERFACES_CONFIG);
}
#line 1047 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 97:
#line 464 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1056 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 103:
#line 478 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the interfaces-config map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1066 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 104:
#line 482 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 1074 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 105:
#line 486 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interfaces", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1085 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 106:
#line 491 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1094 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 107:
#line 496 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.DHCP_SOCKET_TYPE);
}
#line 1102 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 108:
#line 498 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("dhcp-socket-type", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1111 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 109:
#line 503 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("raw", ctx.loc2pos(yystack_[0].location))); }
#line 1117 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 110:
#line 504 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("udp", ctx.loc2pos(yystack_[0].location))); }
#line 1123 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 111:
#line 507 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("re-detect", b);
}
#line 1132 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 112:
#line 513 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lease-database", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.LEASE_DATABASE);
}
#line 1143 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 113:
#line 518 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1152 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 114:
#line 523 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hosts-database", i);
    ctx.stack_.push_back(i);
    ctx.enter(ctx.HOSTS_DATABASE);
}
#line 1163 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 115:
#line 528 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1172 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 131:
#line 552 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.DATABASE_TYPE);
}
#line 1180 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 132:
#line 554 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("type", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1189 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 133:
#line 559 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("memfile", ctx.loc2pos(yystack_[0].location))); }
#line 1195 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 134:
#line 560 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("mysql", ctx.loc2pos(yystack_[0].location))); }
#line 1201 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 135:
#line 561 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("postgresql", ctx.loc2pos(yystack_[0].location))); }
#line 1207 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 136:
#line 562 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("cql", ctx.loc2pos(yystack_[0].location))); }
#line 1213 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 137:
#line 565 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1221 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 138:
#line 567 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr user(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("user", user);
    ctx.leave();
}
#line 1231 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 139:
#line 573 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1239 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 140:
#line 575 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr pwd(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("password", pwd);
    ctx.leave();
}
#line 1249 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 141:
#line 581 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1257 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 142:
#line 583 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr h(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("host", h);
    ctx.leave();
}
#line 1267 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 143:
#line 589 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr p(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("port", p);
}
#line 1276 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 144:
#line 594 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1284 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 145:
#line 596 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr name(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("name", name);
    ctx.leave();
}
#line 1294 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 146:
#line 602 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("persist", n);
}
#line 1303 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 147:
#line 607 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("lfc-interval", n);
}
#line 1312 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 148:
#line 612 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("readonly", n);
}
#line 1321 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 149:
#line 617 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr n(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("connect-timeout", n);
}
#line 1330 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 150:
#line 622 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1338 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 151:
#line 624 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr cp(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("contact-points", cp);
    ctx.leave();
}
#line 1348 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 152:
#line 630 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1356 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 153:
#line 632 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr ks(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("keyspace", ks);
    ctx.leave();
}
#line 1366 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 154:
#line 639 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("host-reservation-identifiers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOST_RESERVATION_IDENTIFIERS);
}
#line 1377 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 155:
#line 644 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1386 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 163:
#line 660 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr duid(new StringElement("duid", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(duid);
}
#line 1395 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 164:
#line 665 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hwaddr(new StringElement("hw-address", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(hwaddr);
}
#line 1404 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 165:
#line 670 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr circuit(new StringElement("circuit-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(circuit);
}
#line 1413 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 166:
#line 675 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr client(new StringElement("client-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(client);
}
#line 1422 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 167:
#line 680 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr flex_id(new StringElement("flex-id", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(flex_id);
}
#line 1431 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 168:
#line 685 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hooks-libraries", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.HOOKS_LIBRARIES);
}
#line 1442 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 169:
#line 690 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1451 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 174:
#line 703 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1461 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 175:
#line 707 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 1469 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 176:
#line 711 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the hooks-libraries list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1479 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 177:
#line 715 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 1487 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 183:
#line 728 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1495 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 184:
#line 730 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr lib(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("library", lib);
    ctx.leave();
}
#line 1505 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 185:
#line 736 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1513 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 186:
#line 738 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("parameters", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1522 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 187:
#line 744 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("expired-leases-processing", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.EXPIRED_LEASES_PROCESSING);
}
#line 1533 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 188:
#line 749 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1542 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 197:
#line 766 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reclaim-timer-wait-time", value);
}
#line 1551 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 198:
#line 771 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush-reclaimed-timer-wait-time", value);
}
#line 1560 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 199:
#line 776 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hold-reclaimed-time", value);
}
#line 1569 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 200:
#line 781 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reclaim-leases", value);
}
#line 1578 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 201:
#line 786 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-reclaim-time", value);
}
#line 1587 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 202:
#line 791 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr value(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("unwarned-reclaim-cycles", value);
}
#line 1596 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 203:
#line 799 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subnet4", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.SUBNET4);
}
#line 1607 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 204:
#line 804 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1616 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 209:
#line 824 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1626 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 210:
#line 828 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Once we reached this place, the subnet parsing is now complete.
    // If we want to, we can implement default values here.
    // In particular we can do things like this:
    // if (!ctx.stack_.back()->get("interface")) {
    //     ctx.stack_.back()->set("interface", StringElement("loopback"));
    // }
    //
    // We can also stack up one level (Dhcp4) and copy over whatever
    // global parameters we want to:
    // if (!ctx.stack_.back()->get("renew-timer")) {
    //     ElementPtr renew = ctx_stack_[...].get("renew-timer");
    //     if (renew) {
    //         ctx.stack_.back()->set("renew-timer", renew);
    //     }
    // }
    ctx.stack_.pop_back();
}
#line 1649 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 211:
#line 847 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the subnet4 list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1659 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 212:
#line 851 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 1667 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 235:
#line 883 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1675 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 236:
#line 885 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr subnet(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("subnet", subnet);
    ctx.leave();
}
#line 1685 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 237:
#line 891 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1693 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 238:
#line 893 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-interface", iface);
    ctx.leave();
}
#line 1703 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 239:
#line 899 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1711 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 240:
#line 901 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-interface-id", iface);
    ctx.leave();
}
#line 1721 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 241:
#line 907 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1729 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 242:
#line 909 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("4o6-subnet", iface);
    ctx.leave();
}
#line 1739 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 243:
#line 915 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1747 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 244:
#line 917 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interface", iface);
    ctx.leave();
}
#line 1757 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 245:
#line 923 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1765 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 246:
#line 925 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr iface(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("interface-id", iface);
    ctx.leave();
}
#line 1775 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 247:
#line 931 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.CLIENT_CLASS);
}
#line 1783 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 248:
#line 933 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr cls(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-class", cls);
    ctx.leave();
}
#line 1793 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 249:
#line 939 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.RESERVATION_MODE);
}
#line 1801 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 250:
#line 941 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("reservation-mode", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 1810 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 251:
#line 946 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("disabled", ctx.loc2pos(yystack_[0].location))); }
#line 1816 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 252:
#line 947 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("out-of-pool", ctx.loc2pos(yystack_[0].location))); }
#line 1822 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 253:
#line 948 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("all", ctx.loc2pos(yystack_[0].location))); }
#line 1828 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 254:
#line 951 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr id(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("id", id);
}
#line 1837 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 255:
#line 956 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr rc(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("rapid-commit", rc);
}
#line 1846 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 256:
#line 963 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("shared-networks", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.SHARED_NETWORK);
}
#line 1857 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 257:
#line 968 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1866 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 262:
#line 983 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1876 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 263:
#line 987 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 1884 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 280:
#line 1015 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("option-def", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OPTION_DEF);
}
#line 1895 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 281:
#line 1020 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 1904 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 286:
#line 1037 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 1914 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 287:
#line 1041 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 1922 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 288:
#line 1048 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the option-def list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 1932 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 289:
#line 1052 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 1940 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 303:
#line 1078 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr code(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("code", code);
}
#line 1949 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 305:
#line 1085 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1957 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 306:
#line 1087 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr prf(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("type", prf);
    ctx.leave();
}
#line 1967 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 307:
#line 1093 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1975 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 308:
#line 1095 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr rtypes(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("record-types", rtypes);
    ctx.leave();
}
#line 1985 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 309:
#line 1101 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 1993 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 310:
#line 1103 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr space(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("space", space);
    ctx.leave();
}
#line 2003 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 312:
#line 1111 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2011 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 313:
#line 1113 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr encap(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("encapsulate", encap);
    ctx.leave();
}
#line 2021 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 314:
#line 1119 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr array(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("array", array);
}
#line 2030 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 315:
#line 1128 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("option-data", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OPTION_DATA);
}
#line 2041 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 316:
#line 1133 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2050 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 321:
#line 1152 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2060 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 322:
#line 1156 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 2068 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 323:
#line 1163 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the option-data list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2078 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 324:
#line 1167 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 2086 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 337:
#line 1197 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2094 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 338:
#line 1199 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr data(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("data", data);
    ctx.leave();
}
#line 2104 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 341:
#line 1209 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr space(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("csv-format", space);
}
#line 2113 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 342:
#line 1214 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr persist(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("always-send", persist);
}
#line 2122 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 343:
#line 1222 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pools", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.POOLS);
}
#line 2133 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 344:
#line 1227 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2142 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 349:
#line 1242 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2152 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 350:
#line 1246 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 2160 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 351:
#line 1250 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the pool list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2170 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 352:
#line 1254 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 2178 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 359:
#line 1268 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2186 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 360:
#line 1270 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr pool(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("pool", pool);
    ctx.leave();
}
#line 2196 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 361:
#line 1276 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2204 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 362:
#line 1278 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("user-context", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2213 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 363:
#line 1286 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("reservations", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.RESERVATIONS);
}
#line 2224 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 364:
#line 1291 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2233 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 369:
#line 1304 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2243 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 370:
#line 1308 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 2251 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 371:
#line 1312 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the reservations list entry map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2261 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 372:
#line 1316 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 2269 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 390:
#line 1344 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2277 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 391:
#line 1346 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr next_server(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("next-server", next_server);
    ctx.leave();
}
#line 2287 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 392:
#line 1352 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2295 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 393:
#line 1354 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr srv(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-hostname", srv);
    ctx.leave();
}
#line 2305 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 394:
#line 1360 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2313 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 395:
#line 1362 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr bootfile(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("boot-file-name", bootfile);
    ctx.leave();
}
#line 2323 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 396:
#line 1368 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2331 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 397:
#line 1370 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr addr(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-address", addr);
    ctx.leave();
}
#line 2341 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 398:
#line 1376 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2349 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 399:
#line 1378 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr d(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("duid", d);
    ctx.leave();
}
#line 2359 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 400:
#line 1384 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2367 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 401:
#line 1386 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hw-address", hw);
    ctx.leave();
}
#line 2377 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 402:
#line 1392 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2385 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 403:
#line 1394 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-id", hw);
    ctx.leave();
}
#line 2395 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 404:
#line 1400 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2403 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 405:
#line 1402 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("circuit-id", hw);
    ctx.leave();
}
#line 2413 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 406:
#line 1408 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2421 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 407:
#line 1410 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr hw(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flex-id", hw);
    ctx.leave();
}
#line 2431 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 408:
#line 1416 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2439 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 409:
#line 1418 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr host(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("hostname", host);
    ctx.leave();
}
#line 2449 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 410:
#line 1424 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr c(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-classes", c);
    ctx.stack_.push_back(c);
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2460 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 411:
#line 1429 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2469 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 412:
#line 1437 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("relay", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.RELAY);
}
#line 2480 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 413:
#line 1442 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2489 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 414:
#line 1447 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2497 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 415:
#line 1449 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr ip(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ip-address", ip);
    ctx.leave();
}
#line 2507 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 416:
#line 1458 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("client-classes", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.CLIENT_CLASSES);
}
#line 2518 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 417:
#line 1463 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2527 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 420:
#line 1472 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 2537 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 421:
#line 1476 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 2545 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 434:
#line 1499 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2553 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 435:
#line 1501 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr test(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("test", test);
    ctx.leave();
}
#line 2563 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 436:
#line 1511 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr time(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp4o6-port", time);
}
#line 2572 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 437:
#line 1518 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("control-socket", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.CONTROL_SOCKET);
}
#line 2583 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 438:
#line 1523 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2592 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 443:
#line 1536 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2600 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 444:
#line 1538 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr stype(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-type", stype);
    ctx.leave();
}
#line 2610 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 445:
#line 1544 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2618 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 446:
#line 1546 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr name(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("socket-name", name);
    ctx.leave();
}
#line 2628 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 447:
#line 1554 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("dhcp-ddns", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.DHCP_DDNS);
}
#line 2639 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 448:
#line 1559 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2648 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 449:
#line 1564 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // Parse the dhcp-ddns map
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.push_back(m);
}
#line 2658 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 450:
#line 1568 "dhcp4_parser.yy" // lalr1.cc:859
    {
    // parsing completed
}
#line 2666 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 468:
#line 1593 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("enable-updates", b);
}
#line 2675 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 469:
#line 1598 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2683 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 470:
#line 1600 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("qualifying-suffix", s);
    ctx.leave();
}
#line 2693 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 471:
#line 1606 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2701 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 472:
#line 1608 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-ip", s);
    ctx.leave();
}
#line 2711 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 473:
#line 1614 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("server-port", i);
}
#line 2720 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 474:
#line 1619 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2728 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 475:
#line 1621 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sender-ip", s);
    ctx.leave();
}
#line 2738 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 476:
#line 1627 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("sender-port", i);
}
#line 2747 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 477:
#line 1632 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr i(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("max-queue-size", i);
}
#line 2756 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 478:
#line 1637 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NCR_PROTOCOL);
}
#line 2764 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 479:
#line 1639 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("ncr-protocol", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2773 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 480:
#line 1645 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("UDP", ctx.loc2pos(yystack_[0].location))); }
#line 2779 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 481:
#line 1646 "dhcp4_parser.yy" // lalr1.cc:859
    { yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("TCP", ctx.loc2pos(yystack_[0].location))); }
#line 2785 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 482:
#line 1649 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NCR_FORMAT);
}
#line 2793 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 483:
#line 1651 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr json(new StringElement("JSON", ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("ncr-format", json);
    ctx.leave();
}
#line 2803 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 484:
#line 1657 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("always-include-fqdn", b);
}
#line 2812 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 485:
#line 1662 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("override-no-update", b);
}
#line 2821 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 486:
#line 1667 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr b(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("override-client-update", b);
}
#line 2830 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 487:
#line 1672 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.REPLACE_CLIENT_NAME);
}
#line 2838 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 488:
#line 1674 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("replace-client-name", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2847 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 489:
#line 1680 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("when-present", ctx.loc2pos(yystack_[0].location))); 
      }
#line 2855 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 490:
#line 1683 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("never", ctx.loc2pos(yystack_[0].location)));
      }
#line 2863 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 491:
#line 1686 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("always", ctx.loc2pos(yystack_[0].location)));
      }
#line 2871 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 492:
#line 1689 "dhcp4_parser.yy" // lalr1.cc:859
    {
      yylhs.value.as< ElementPtr > () = ElementPtr(new StringElement("when-not-present", ctx.loc2pos(yystack_[0].location)));
      }
#line 2879 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 493:
#line 1692 "dhcp4_parser.yy" // lalr1.cc:859
    {
      error(yystack_[0].location, "boolean values for the replace-client-name are "
                "no longer supported");
      }
#line 2888 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 494:
#line 1698 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2896 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 495:
#line 1700 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr s(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("generated-prefix", s);
    ctx.leave();
}
#line 2906 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 496:
#line 1708 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2914 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 497:
#line 1710 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("Dhcp6", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2923 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 498:
#line 1715 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2931 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 499:
#line 1717 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("DhcpDdns", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2940 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 500:
#line 1722 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 2948 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 501:
#line 1724 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.back()->set("Control-agent", yystack_[0].value.as< ElementPtr > ());
    ctx.leave();
}
#line 2957 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 502:
#line 1734 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("Logging", m);
    ctx.stack_.push_back(m);
    ctx.enter(ctx.LOGGING);
}
#line 2968 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 503:
#line 1739 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2977 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 507:
#line 1756 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("loggers", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.LOGGERS);
}
#line 2988 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 508:
#line 1761 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 2997 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 511:
#line 1773 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(l);
    ctx.stack_.push_back(l);
}
#line 3007 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 512:
#line 1777 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 3015 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 520:
#line 1792 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr dl(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("debuglevel", dl);
}
#line 3024 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 521:
#line 1797 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3032 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 522:
#line 1799 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr sev(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("severity", sev);
    ctx.leave();
}
#line 3042 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 523:
#line 1805 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr l(new ListElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output_options", l);
    ctx.stack_.push_back(l);
    ctx.enter(ctx.OUTPUT_OPTIONS);
}
#line 3053 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 524:
#line 1810 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
    ctx.leave();
}
#line 3062 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 527:
#line 1819 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr m(new MapElement(ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->add(m);
    ctx.stack_.push_back(m);
}
#line 3072 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 528:
#line 1823 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.stack_.pop_back();
}
#line 3080 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 535:
#line 1837 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ctx.enter(ctx.NO_KEYWORD);
}
#line 3088 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 536:
#line 1839 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr sev(new StringElement(yystack_[0].value.as< std::string > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("output", sev);
    ctx.leave();
}
#line 3098 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 537:
#line 1845 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr flush(new BoolElement(yystack_[0].value.as< bool > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("flush", flush);
}
#line 3107 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 538:
#line 1850 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr maxsize(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxsize", maxsize);
}
#line 3116 "dhcp4_parser.cc" // lalr1.cc:859
    break;

  case 539:
#line 1855 "dhcp4_parser.yy" // lalr1.cc:859
    {
    ElementPtr maxver(new IntElement(yystack_[0].value.as< int64_t > (), ctx.loc2pos(yystack_[0].location)));
    ctx.stack_.back()->set("maxver", maxver);
}
#line 3125 "dhcp4_parser.cc" // lalr1.cc:859
    break;


#line 3129 "dhcp4_parser.cc" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
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

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  Dhcp4Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  Dhcp4Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

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
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int Dhcp4Parser::yypact_ninf_ = -673;

  const signed char Dhcp4Parser::yytable_ninf_ = -1;

  const short int
  Dhcp4Parser::yypact_[] =
  {
     217,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,    28,    20,    32,    55,    76,    87,   116,   126,
     132,   152,   159,   183,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,    20,   -91,    16,    26,
      80,    13,   143,   106,   118,    50,   -30,   235,  -673,    58,
     148,   105,   197,   204,  -673,  -673,  -673,  -673,  -673,   211,
    -673,    27,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   223,
     234,  -673,  -673,  -673,   240,   251,   273,   274,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,   282,  -673,  -673,  -673,
      78,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,   291,   114,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   292,
     295,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
     128,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,   169,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,   260,   239,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,   299,  -673,  -673,  -673,   300,  -673,  -673,
     267,   302,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,   303,   304,  -673,  -673,  -673,  -673,   305,
     307,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,   173,  -673,  -673,  -673,   308,  -673,  -673,   317,  -673,
     318,   319,  -673,  -673,   320,   321,   322,  -673,  -673,  -673,
     203,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,    20,    20,  -673,
     185,   324,   325,   327,   328,   355,  -673,    16,  -673,   359,
     219,   221,   361,   363,   364,   226,   227,   228,   229,   369,
     370,   371,   372,   374,   375,   376,   377,   241,   378,   379,
      26,  -673,   381,   382,   242,    80,  -673,    70,   384,   385,
     386,   387,   388,   389,   390,   252,   253,   392,   393,   395,
     396,    13,  -673,   397,   398,   143,  -673,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,  -673,   106,   409,
     410,   272,   412,   413,   414,   275,  -673,   118,   415,   276,
     277,  -673,    50,   419,   420,   -15,  -673,   280,   422,   423,
     285,   425,   287,   288,   428,   429,   289,   290,   293,   432,
     433,   235,  -673,  -673,  -673,   435,   434,   437,    20,    20,
      20,  -673,   438,  -673,  -673,   298,   439,   440,  -673,  -673,
    -673,  -673,   445,   446,   449,   450,   451,   452,   453,   454,
    -673,   455,   456,  -673,   459,   165,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,   457,   463,  -673,  -673,  -673,
     326,   330,   331,   464,   332,   333,   334,  -673,  -673,   247,
     335,   465,   472,  -673,   338,   474,  -673,   340,   341,   459,
     342,   343,   345,   346,   347,   348,   349,  -673,   350,   351,
    -673,   352,   353,   354,  -673,  -673,   356,  -673,  -673,  -673,
     357,    20,  -673,  -673,   358,   360,  -673,   362,  -673,  -673,
      21,   383,  -673,  -673,  -673,   102,   365,  -673,    20,    26,
     391,  -673,  -673,  -673,    80,  -673,   164,   164,   494,   496,
     498,   499,   175,    31,   501,   144,   133,   235,  -673,  -673,
    -673,  -673,  -673,   505,  -673,    70,  -673,  -673,  -673,   503,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   506,   431,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,   206,  -673,   218,  -673,
    -673,   220,  -673,  -673,  -673,  -673,   508,   511,   512,   513,
     514,  -673,  -673,  -673,   237,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   238,  -673,
     436,   516,  -673,  -673,   515,   517,  -673,  -673,   518,   519,
    -673,  -673,   520,   522,  -673,  -673,  -673,  -673,  -673,  -673,
      44,  -673,  -673,  -673,  -673,  -673,  -673,  -673,    57,  -673,
     521,   525,  -673,   526,   527,   528,   529,   530,   531,   254,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   255,
    -673,  -673,  -673,   256,   394,   411,  -673,  -673,   523,   534,
    -673,  -673,   532,   536,  -673,  -673,   533,  -673,   538,   391,
    -673,  -673,   539,   540,   541,   542,   380,   416,   417,   418,
     421,   543,   544,   164,  -673,  -673,    13,  -673,   494,   118,
    -673,   496,    50,  -673,   498,    89,  -673,   499,   175,  -673,
      31,  -673,   -30,  -673,   501,   424,   426,   427,   430,   441,
     442,   144,  -673,   545,   546,   133,  -673,  -673,  -673,   548,
     537,  -673,   143,  -673,   503,   106,  -673,   506,   547,  -673,
     550,  -673,   265,   444,   447,   448,  -673,  -673,  -673,  -673,
    -673,   458,   460,  -673,   258,  -673,   549,  -673,   551,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
     262,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   264,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   461,   462,
    -673,  -673,   466,   268,  -673,   554,  -673,   467,   558,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,    89,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,    79,  -673,  -673,   101,   558,  -673,  -673,   552,
    -673,  -673,  -673,   271,  -673,  -673,  -673,  -673,  -673,   562,
     468,   564,   101,  -673,   553,  -673,   470,  -673,   565,  -673,
    -673,   154,  -673,   -11,   565,  -673,  -673,   567,   570,   571,
     281,  -673,  -673,  -673,  -673,  -673,  -673,   572,   443,   471,
     473,   -11,  -673,   475,  -673,  -673,  -673,  -673,  -673
  };

  const unsigned short int
  Dhcp4Parser::yydefact_[] =
  {
       0,     2,     4,     6,     8,    10,    12,    14,    16,    18,
      20,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     1,    39,    32,    28,    27,    24,
      25,    26,    31,     3,    29,    30,    52,     5,    64,     7,
     103,     9,   211,    11,   351,    13,   371,    15,   288,    17,
     323,    19,   176,    21,   449,    23,    41,    35,     0,     0,
       0,     0,     0,   373,   290,   325,     0,     0,    43,     0,
      42,     0,     0,    36,    62,   502,   496,   498,   500,     0,
      61,     0,    54,    56,    58,    59,    60,    57,    96,     0,
       0,   390,   112,   114,     0,     0,     0,     0,   203,   280,
     315,   256,   154,   416,   168,   187,     0,   437,   447,    89,
       0,    66,    68,    69,    70,    71,    86,    87,    74,    75,
      76,    77,    81,    82,    72,    73,    79,    80,    88,    78,
      83,    84,    85,   105,   107,     0,     0,    98,   100,   101,
     102,   420,   237,   239,   241,   343,   235,   243,   245,     0,
       0,   249,   247,   363,   412,   234,   215,   216,   217,   229,
       0,   213,   220,   231,   232,   233,   221,   222,   225,   227,
     223,   224,   218,   219,   226,   230,   228,   359,   361,   358,
     356,     0,   353,   355,   357,   392,   394,   410,   398,   400,
     404,   402,   408,   406,   396,   389,   385,     0,   374,   375,
     386,   387,   388,   382,   377,   383,   379,   380,   381,   384,
     378,   305,   144,     0,   309,   307,   312,     0,   301,   302,
       0,   291,   292,   294,   304,   295,   296,   297,   311,   298,
     299,   300,   337,     0,     0,   335,   336,   339,   340,     0,
     326,   327,   329,   330,   331,   332,   333,   334,   183,   185,
     180,     0,   178,   181,   182,     0,   469,   471,     0,   474,
       0,     0,   478,   482,     0,     0,     0,   487,   494,   467,
       0,   451,   453,   454,   455,   456,   457,   458,   459,   460,
     461,   462,   463,   464,   465,   466,    40,     0,     0,    33,
       0,     0,     0,     0,     0,     0,    51,     0,    53,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    65,     0,     0,     0,     0,   104,   422,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,     0,     0,     0,   352,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   372,     0,     0,
       0,     0,     0,     0,     0,     0,   289,     0,     0,     0,
       0,   324,     0,     0,     0,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   450,    44,    37,     0,     0,     0,     0,     0,
       0,    55,     0,    94,    95,     0,     0,     0,    90,    91,
      92,    93,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,    67,     0,     0,   111,    99,   434,   432,
     433,   428,   429,   430,   431,     0,   423,   424,   426,   427,
       0,     0,     0,     0,     0,     0,     0,   254,   255,     0,
       0,     0,     0,   214,     0,     0,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   376,     0,     0,
     303,     0,     0,     0,   314,   293,     0,   341,   342,   328,
       0,     0,   179,   468,     0,     0,   473,     0,   476,   477,
       0,     0,   484,   485,   486,     0,     0,   452,     0,     0,
       0,   497,   499,   501,     0,   391,     0,     0,   205,   282,
     317,   258,     0,     0,   170,     0,     0,     0,    45,   106,
     109,   110,   108,     0,   421,     0,   238,   240,   242,   345,
     236,   244,   246,   251,   252,   253,   250,   248,   365,     0,
     360,    34,   362,   393,   395,   411,   399,   401,   405,   403,
     409,   407,   397,   306,   145,   310,   308,   313,   338,   184,
     186,   470,   472,   475,   480,   481,   479,   483,   489,   490,
     491,   492,   493,   488,   495,    38,     0,   507,     0,   504,
     506,     0,   131,   137,   139,   141,     0,     0,     0,     0,
       0,   150,   152,   130,     0,   116,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,     0,   209,
       0,   206,   207,   286,     0,   283,   284,   321,     0,   318,
     319,   262,     0,   259,   260,   163,   164,   165,   166,   167,
       0,   156,   158,   159,   160,   161,   162,   418,     0,   174,
       0,   171,   172,     0,     0,     0,     0,     0,     0,     0,
     189,   191,   192,   193,   194,   195,   196,   443,   445,     0,
     439,   441,   442,     0,    47,     0,   425,   349,     0,   346,
     347,   369,     0,   366,   367,   414,     0,    63,     0,     0,
     503,    97,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   113,   115,     0,   204,     0,   290,
     281,     0,   325,   316,     0,     0,   257,     0,     0,   155,
       0,   417,     0,   169,     0,     0,     0,     0,     0,     0,
       0,     0,   188,     0,     0,     0,   438,   448,    49,     0,
      48,   435,     0,   344,     0,   373,   364,     0,     0,   413,
       0,   505,     0,     0,     0,     0,   143,   146,   147,   148,
     149,     0,     0,   117,     0,   208,     0,   285,     0,   320,
     279,   278,   269,   270,   276,   272,   266,   267,   268,   275,
       0,   264,   271,   273,   274,   277,   261,   157,   419,     0,
     173,   197,   198,   199,   200,   201,   202,   190,     0,     0,
     440,    46,     0,     0,   348,     0,   368,     0,     0,   133,
     134,   135,   136,   132,   138,   140,   142,   151,   153,   210,
     287,   322,     0,   263,   175,   444,   446,    50,   350,   370,
     415,   511,     0,   509,   265,     0,     0,   508,   523,     0,
     521,   519,   515,     0,   513,   517,   518,   516,   510,     0,
       0,     0,     0,   512,     0,   520,     0,   514,     0,   522,
     527,     0,   525,     0,     0,   524,   535,     0,     0,     0,
       0,   529,   531,   532,   533,   534,   526,     0,     0,     0,
       0,     0,   528,     0,   537,   538,   539,   530,   536
  };

  const short int
  Dhcp4Parser::yypgoto_[] =
  {
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,   -38,  -673,    97,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,   119,  -673,  -673,  -673,   -58,  -673,
    -673,  -673,   283,  -673,  -673,  -673,  -673,    82,   257,   -60,
     -59,   -44,  -673,  -672,   -40,  -673,  -673,    75,   266,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,    85,  -111,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,   -54,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -125,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -127,  -673,  -673,  -673,
    -119,   222,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -126,
    -673,  -673,  -673,  -673,  -673,  -673,  -671,  -673,  -673,  -673,
    -104,  -673,  -673,  -673,  -100,   269,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -665,  -673,  -673,  -673,  -494,  -673,
    -664,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -109,
    -673,  -673,  -213,  -673,  -673,  -673,  -673,   -96,  -673,  -673,
    -673,   -98,  -673,   246,  -673,   -52,  -673,  -673,  -673,  -673,
    -673,   -42,  -673,  -673,  -673,  -673,  -673,   -47,  -673,  -673,
    -673,   -97,  -673,  -673,  -673,   -87,  -673,   248,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -128,
    -673,  -673,  -673,  -114,   278,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -118,  -673,  -673,  -673,  -113,  -673,   263,
     -39,  -673,  -317,  -673,  -315,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -663,  -673,  -673,  -673,  -659,  -673,  -673,  -673,
    -673,  -673,    99,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
     -99,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,   108,
     236,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,  -673,
    -673,  -673,  -673,   -51,  -673,  -673,  -673,  -197,  -673,  -673,
    -212,  -673,  -673,  -673,  -673,  -673,  -673,  -223,  -673,  -673,
    -239,  -673,  -673,  -673,  -673,  -673
  };

  const short int
  Dhcp4Parser::yydefgoto_[] =
  {
      -1,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    32,    33,    34,    57,   542,    72,    73,
      35,    56,    69,    70,   519,   664,   729,   730,   109,    37,
      58,    81,    82,    83,   291,    39,    59,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   299,   136,   137,    41,
      60,   138,   322,   139,   323,   522,   140,   119,   303,   120,
     304,   594,   595,   596,   682,   803,   597,   683,   598,   684,
     599,   685,   600,   219,   360,   602,   603,   604,   605,   606,
     691,   607,   692,   121,   313,   630,   631,   632,   633,   634,
     635,   636,   122,   315,   640,   641,   642,   712,    53,    66,
     251,   252,   253,   373,   254,   374,   123,   316,   649,   650,
     651,   652,   653,   654,   655,   656,   124,   309,   610,   611,
     612,   696,    43,    61,   160,   161,   162,   332,   163,   328,
     164,   329,   165,   330,   166,   333,   167,   334,   168,   338,
     169,   337,   536,   170,   171,   125,   312,   622,   623,   624,
     705,   770,   771,   126,   310,   614,   615,   616,   699,    49,
      64,   220,   221,   222,   223,   224,   225,   226,   359,   227,
     363,   228,   362,   229,   230,   364,   231,   127,   311,   618,
     619,   620,   702,    51,    65,   239,   240,   241,   242,   243,
     368,   244,   245,   246,   247,   173,   331,   668,   669,   670,
     732,    45,    62,   181,   182,   183,   343,   184,   344,   174,
     339,   672,   673,   674,   735,    47,    63,   197,   198,   199,
     128,   302,   201,   347,   202,   348,   203,   356,   204,   350,
     205,   351,   206,   353,   207,   352,   208,   355,   209,   354,
     210,   349,   176,   340,   676,   738,   129,   314,   638,   327,
     435,   436,   437,   438,   439,   523,   130,   131,   318,   659,
     660,   661,   723,   662,   724,   132,   319,    55,    67,   270,
     271,   272,   273,   378,   274,   379,   275,   276,   381,   277,
     278,   279,   384,   566,   280,   385,   281,   282,   283,   284,
     389,   573,   285,   390,    84,   293,    85,   294,    86,   295,
      87,   292,   578,   579,   580,   678,   822,   823,   825,   833,
     834,   835,   836,   841,   837,   839,   851,   852,   853,   860,
     861,   862,   867,   863,   864,   865
  };

  const unsigned short int
  Dhcp4Parser::yytable_[] =
  {
      80,   156,   157,   155,   179,   195,   218,   235,   250,   269,
     433,   236,   434,   237,   172,   180,   196,   158,    68,   637,
     141,   159,   175,   238,   200,    25,    74,    26,    24,    27,
     297,    90,    91,   764,   767,   298,   564,    88,   141,    36,
     768,   769,   774,    89,    90,    91,   775,   708,    92,    93,
     709,    71,    94,    95,    96,   248,   249,   142,   143,   144,
     710,   100,    38,   711,   286,    94,    95,    96,    97,    98,
     248,   249,   145,    99,   100,   146,   147,   148,   149,   150,
     151,   320,   826,    40,   101,   827,   321,   152,   153,    91,
     185,   186,   133,   134,    42,   154,   135,   102,   103,   212,
     232,   213,   214,   233,   234,   152,    89,    90,    91,   288,
     104,   856,    79,   105,   857,   858,   859,   325,   100,   212,
     106,   107,   326,    44,   108,    91,   185,   186,    94,    95,
      96,   341,    98,    46,   565,    75,   342,   100,   212,    48,
     764,   767,   211,   428,    76,    77,    78,   768,   769,   774,
     212,   287,   147,   775,   100,    79,   151,   854,    79,    50,
     855,   103,    28,    29,    30,    31,    52,   212,    79,   213,
     214,   154,   345,   215,   216,   217,   375,   346,   187,   520,
     521,   376,   188,   189,   190,   191,   192,   193,   582,   194,
      54,   100,    79,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   177,   178,   289,   391,   290,   433,   320,
     434,   392,    79,   212,   677,   296,   778,   568,   569,   570,
     571,   679,   828,   325,   829,   830,   680,   300,   681,   657,
     658,    79,   643,   644,   645,   646,   647,   648,   301,    80,
     693,   693,   358,    79,   305,   694,   695,   572,    79,   393,
     394,   625,   626,   627,   628,   306,   629,   721,   725,   391,
      79,   341,   722,   726,   727,   812,   809,   375,   357,   429,
     813,   345,   814,   430,   842,   366,   818,   307,   308,   843,
     431,   156,   157,   155,   871,    79,   317,   179,   432,   872,
     799,   800,   801,   802,   172,   324,   335,   158,   180,   336,
     195,   159,   175,   361,   365,   367,    79,   369,   370,   218,
     372,   196,   377,   371,   235,   533,   534,   535,   236,   200,
     237,   380,   382,   383,   386,   387,   388,   395,   396,   397,
     238,   398,   399,   269,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   267,   268,     1,     2,
       3,     4,     5,     6,     7,     8,     9,    10,    11,   400,
     501,   502,   503,   402,   403,   405,   404,   406,   407,   408,
     409,   410,   411,   412,   413,   414,   415,    79,   416,   417,
     418,   419,   421,   422,   420,   424,   425,   426,   440,   441,
     442,   443,   444,   445,   446,   447,   449,   450,   448,   451,
     452,   454,   455,   457,   458,   459,   460,   461,   462,   463,
     464,   465,   466,   468,   469,   470,   471,   472,   473,   476,
     474,   477,   478,   480,   481,   483,   484,   485,   486,   487,
     488,   489,   490,   491,   492,   493,   495,   496,   494,   498,
     505,   499,   697,   560,   500,   504,   506,   507,   593,   593,
     508,   509,   601,   601,   510,   511,   512,   513,   514,   269,
     575,   515,   516,   517,   518,   524,   525,   429,   526,   529,
     538,   430,   527,   528,   530,   531,   532,   537,   431,   539,
     540,    26,   543,   544,   546,   547,   432,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   567,   558,   559,
     561,   609,   562,   613,   563,   617,   621,   574,   639,   665,
     667,   577,   686,   671,   675,   687,   688,   689,   690,   698,
     701,   700,   704,   746,   703,   707,   706,   713,   714,   733,
     715,   716,   717,   718,   719,   720,   728,   734,   736,   737,
     792,   739,   740,   742,   743,   744,   745,   751,   752,   788,
     789,   797,   541,   731,   791,   798,   840,   810,   848,   811,
     748,   747,   819,   749,   750,   821,   844,   781,   846,   782,
     783,   868,   850,   784,   869,   870,   873,   423,   545,   581,
     401,   576,   753,   777,   785,   786,   804,   780,   874,   805,
     806,   427,   608,   779,   755,   787,   754,   482,   776,   824,
     807,   756,   808,   815,   816,   757,   794,   759,   817,   820,
     453,   845,   849,   475,   875,   758,   876,   878,   793,   796,
     479,   467,   795,   456,   666,   663,   790,   497,   741,   838,
     847,   866,   877,     0,     0,   593,   156,   157,   155,   601,
       0,   218,     0,     0,   235,   761,   762,   760,   236,   172,
     237,   766,   158,     0,   250,     0,   159,   175,   772,     0,
     238,   763,     0,     0,     0,   765,   773,     0,     0,     0,
       0,     0,     0,     0,   179,     0,     0,   195,     0,     0,
       0,     0,     0,     0,     0,   180,     0,     0,   196,     0,
       0,     0,     0,     0,     0,     0,   200,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   761,   762,   760,     0,     0,     0,   766,     0,
       0,     0,     0,     0,     0,   772,     0,   831,   763,     0,
       0,   832,   765,   773,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   831,     0,     0,     0,   832
  };

  const short int
  Dhcp4Parser::yycheck_[] =
  {
      58,    61,    61,    61,    62,    63,    64,    65,    66,    67,
     327,    65,   327,    65,    61,    62,    63,    61,    56,   513,
       7,    61,    61,    65,    63,     5,    10,     7,     0,     9,
       3,    18,    19,   705,   705,     8,    15,    11,     7,     7,
     705,   705,   705,    17,    18,    19,   705,     3,    22,    23,
       6,   142,    39,    40,    41,    85,    86,    44,    45,    46,
       3,    48,     7,     6,     6,    39,    40,    41,    42,    43,
      85,    86,    59,    47,    48,    62,    63,    64,    65,    66,
      67,     3,     3,     7,    58,     6,     8,    74,    75,    19,
      20,    21,    12,    13,     7,    82,    16,    71,    72,    49,
      50,    51,    52,    53,    54,    74,    17,    18,    19,     4,
      84,   122,   142,    87,   125,   126,   127,     3,    48,    49,
      94,    95,     8,     7,    98,    19,    20,    21,    39,    40,
      41,     3,    43,     7,   113,   119,     8,    48,    49,     7,
     812,   812,    24,    73,   128,   129,   130,   812,   812,   812,
      49,     3,    63,   812,    48,   142,    67,     3,   142,     7,
       6,    72,   142,   143,   144,   145,     7,    49,   142,    51,
      52,    82,     3,    55,    56,    57,     3,     8,    72,    14,
      15,     8,    76,    77,    78,    79,    80,    81,    24,    83,
       7,    48,   142,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    60,    61,     8,     3,     3,   525,     3,
     525,     8,   142,    49,     8,     4,   710,   115,   116,   117,
     118,     3,   121,     3,   123,   124,     8,     4,     8,    96,
      97,   142,    88,    89,    90,    91,    92,    93,     4,   297,
       3,     3,     3,   142,     4,     8,     8,   145,   142,   287,
     288,    76,    77,    78,    79,     4,    81,     3,     3,     3,
     142,     3,     8,     8,     8,     3,     8,     3,     8,   327,
       8,     3,     8,   327,     3,     8,     8,     4,     4,     8,
     327,   341,   341,   341,     3,   142,     4,   345,   327,     8,
      25,    26,    27,    28,   341,     4,     4,   341,   345,     4,
     358,   341,   341,     4,     4,     3,   142,     4,     4,   367,
       3,   358,     4,     8,   372,    68,    69,    70,   372,   358,
     372,     4,     4,     4,     4,     4,     4,   142,     4,     4,
     372,     4,     4,   391,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,     4,
     398,   399,   400,     4,   145,     4,   145,     4,     4,   143,
     143,   143,   143,     4,     4,     4,     4,   142,     4,     4,
       4,     4,     4,     4,   143,     4,     4,   145,     4,     4,
       4,     4,     4,     4,     4,   143,     4,     4,   145,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,   143,     4,     4,     4,     4,
     145,   145,   145,     4,     4,   145,     4,     4,   143,     4,
     143,   143,     4,     4,   145,   145,     4,     4,   145,     4,
     142,     7,     6,   481,     7,     7,     7,     7,   506,   507,
       5,     5,   506,   507,     5,     5,     5,     5,     5,   517,
     498,     7,     7,     7,     5,     8,     3,   525,   142,     5,
       5,   525,   142,   142,   142,   142,   142,   142,   525,     7,
     142,     7,   142,   142,   142,   142,   525,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   114,   142,   142,
     142,     7,   142,     7,   142,     7,     7,   142,     7,     4,
       7,   120,     4,     7,    83,     4,     4,     4,     4,     3,
       3,     6,     3,   143,     6,     3,     6,     6,     3,     6,
       4,     4,     4,     4,     4,     4,   142,     3,     6,     3,
       3,     8,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,   455,   142,     6,     5,     4,     8,     5,     8,
     143,   145,     8,   145,   143,     7,     4,   143,     4,   143,
     143,     4,     7,   143,     4,     4,     4,   320,   459,   504,
     297,   499,   693,   708,   143,   143,   142,   714,   145,   142,
     142,   325,   507,   712,   698,   721,   696,   375,   707,   812,
     142,   699,   142,   142,   142,   701,   734,   704,   142,   142,
     341,   143,   142,   367,   143,   702,   143,   142,   732,   737,
     372,   358,   735,   345,   525,   517,   725,   391,   679,   826,
     842,   854,   871,    -1,    -1,   693,   696,   696,   696,   693,
      -1,   699,    -1,    -1,   702,   705,   705,   705,   702,   696,
     702,   705,   696,    -1,   712,    -1,   696,   696,   705,    -1,
     702,   705,    -1,    -1,    -1,   705,   705,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   732,    -1,    -1,   735,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   732,    -1,    -1,   735,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   735,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   812,   812,   812,    -1,    -1,    -1,   812,    -1,
      -1,    -1,    -1,    -1,    -1,   812,    -1,   825,   812,    -1,
      -1,   825,   812,   812,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   842,    -1,    -1,    -1,   842
  };

  const unsigned short int
  Dhcp4Parser::yystos_[] =
  {
       0,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,     0,     5,     7,     9,   142,   143,
     144,   145,   159,   160,   161,   166,     7,   175,     7,   181,
       7,   195,     7,   268,     7,   347,     7,   361,     7,   305,
       7,   329,     7,   244,     7,   413,   167,   162,   176,   182,
     196,   269,   348,   362,   306,   330,   245,   414,   159,   168,
     169,   142,   164,   165,    10,   119,   128,   129,   130,   142,
     174,   177,   178,   179,   440,   442,   444,   446,    11,    17,
      18,    19,    22,    23,    39,    40,    41,    42,    43,    47,
      48,    58,    71,    72,    84,    87,    94,    95,    98,   174,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   203,
     205,   229,   238,   252,   262,   291,   299,   323,   366,   392,
     402,   403,   411,    12,    13,    16,   193,   194,   197,   199,
     202,     7,    44,    45,    46,    59,    62,    63,    64,    65,
      66,    67,    74,    75,    82,   174,   185,   186,   187,   190,
     270,   271,   272,   274,   276,   278,   280,   282,   284,   286,
     289,   290,   323,   341,   355,   366,   388,    60,    61,   174,
     323,   349,   350,   351,   353,    20,    21,    72,    76,    77,
      78,    79,    80,    81,    83,   174,   323,   363,   364,   365,
     366,   368,   370,   372,   374,   376,   378,   380,   382,   384,
     386,    24,    49,    51,    52,    55,    56,    57,   174,   219,
     307,   308,   309,   310,   311,   312,   313,   315,   317,   319,
     320,   322,    50,    53,    54,   174,   219,   311,   317,   331,
     332,   333,   334,   335,   337,   338,   339,   340,    85,    86,
     174,   246,   247,   248,   250,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   174,
     415,   416,   417,   418,   420,   422,   423,   425,   426,   427,
     430,   432,   433,   434,   435,   438,     6,     3,     4,     8,
       3,   180,   447,   441,   443,   445,     4,     3,     8,   192,
       4,     4,   367,   204,   206,     4,     4,     4,     4,   263,
     300,   324,   292,   230,   393,   239,   253,     4,   404,   412,
       3,     8,   198,   200,     4,     3,     8,   395,   275,   277,
     279,   342,   273,   281,   283,     4,     4,   287,   285,   356,
     389,     3,     8,   352,   354,     3,     8,   369,   371,   387,
     375,   377,   381,   379,   385,   383,   373,     8,     3,   314,
     220,     4,   318,   316,   321,     4,     8,     3,   336,     4,
       4,     8,     3,   249,   251,     3,     8,     4,   419,   421,
       4,   424,     4,     4,   428,   431,     4,     4,     4,   436,
     439,     3,     8,   159,   159,   142,     4,     4,     4,     4,
       4,   178,     4,   145,   145,     4,     4,     4,   143,   143,
     143,   143,     4,     4,     4,     4,     4,     4,     4,     4,
     143,     4,     4,   184,     4,     4,   145,   194,    73,   174,
     219,   323,   366,   368,   370,   396,   397,   398,   399,   400,
       4,     4,     4,     4,     4,     4,     4,   143,   145,     4,
       4,     4,     4,   271,     4,     4,   350,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,   365,     4,     4,
     143,     4,     4,     4,   145,   309,     4,   145,   145,   333,
       4,     4,   247,   145,     4,     4,   143,     4,   143,   143,
       4,     4,   145,   145,   145,     4,     4,   416,     4,     7,
       7,   159,   159,   159,     7,   142,     7,     7,     5,     5,
       5,     5,     5,     5,     5,     7,     7,     7,     5,   170,
      14,    15,   201,   401,     8,     3,   142,   142,   142,     5,
     142,   142,   142,    68,    69,    70,   288,   142,     5,     7,
     142,   161,   163,   142,   142,   170,   142,   142,   142,   142,
     142,   142,   142,   142,   142,   142,   142,   142,   142,   142,
     159,   142,   142,   142,    15,   113,   429,   114,   115,   116,
     117,   118,   145,   437,   142,   159,   183,   120,   448,   449,
     450,   193,    24,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,   174,   207,   208,   209,   212,   214,   216,
     218,   219,   221,   222,   223,   224,   225,   227,   207,     7,
     264,   265,   266,     7,   301,   302,   303,     7,   325,   326,
     327,     7,   293,   294,   295,    76,    77,    78,    79,    81,
     231,   232,   233,   234,   235,   236,   237,   284,   394,     7,
     240,   241,   242,    88,    89,    90,    91,    92,    93,   254,
     255,   256,   257,   258,   259,   260,   261,    96,    97,   405,
     406,   407,   409,   415,   171,     4,   398,     7,   343,   344,
     345,     7,   357,   358,   359,    83,   390,     8,   451,     3,
       8,     8,   210,   213,   215,   217,     4,     4,     4,     4,
       4,   226,   228,     3,     8,     8,   267,     6,     3,   304,
       6,     3,   328,     6,     3,   296,     6,     3,     3,     6,
       3,     6,   243,     6,     3,     4,     4,     4,     4,     4,
       4,     3,     8,   408,   410,     3,     8,     8,   142,   172,
     173,   142,   346,     6,     3,   360,     6,     3,   391,     8,
       4,   449,     4,     4,     4,     4,   143,   145,   143,   145,
     143,     4,     4,   208,   270,   266,   307,   303,   331,   327,
     174,   185,   186,   187,   189,   190,   219,   262,   280,   286,
     297,   298,   323,   366,   388,   392,   295,   232,   284,   246,
     242,   143,   143,   143,   143,   143,   143,   255,     4,     4,
     406,     6,     3,   349,   345,   363,   359,     4,     5,    25,
      26,    27,    28,   211,   142,   142,   142,   142,   142,     8,
       8,     8,     3,     8,     8,   142,   142,   142,     8,     8,
     142,     7,   452,   453,   298,   454,     3,     6,   121,   123,
     124,   174,   219,   455,   456,   457,   458,   460,   453,   461,
       4,   459,     3,     8,     4,   143,     4,   456,     5,   142,
       7,   462,   463,   464,     3,     6,   122,   125,   126,   127,
     465,   466,   467,   469,   470,   471,   463,   468,     4,     4,
       4,     3,     8,     4,   145,   143,   143,   466,   142
  };

  const unsigned short int
  Dhcp4Parser::yyr1_[] =
  {
       0,   146,   148,   147,   149,   147,   150,   147,   151,   147,
     152,   147,   153,   147,   154,   147,   155,   147,   156,   147,
     157,   147,   158,   147,   159,   159,   159,   159,   159,   159,
     159,   160,   162,   161,   163,   164,   164,   165,   165,   167,
     166,   168,   168,   169,   169,   171,   170,   172,   172,   173,
     173,   174,   176,   175,   177,   177,   178,   178,   178,   178,
     178,   178,   180,   179,   182,   181,   183,   183,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     184,   184,   184,   184,   184,   184,   184,   184,   184,   184,
     185,   186,   187,   188,   189,   190,   192,   191,   193,   193,
     194,   194,   194,   196,   195,   198,   197,   200,   199,   201,
     201,   202,   204,   203,   206,   205,   207,   207,   208,   208,
     208,   208,   208,   208,   208,   208,   208,   208,   208,   208,
     208,   210,   209,   211,   211,   211,   211,   213,   212,   215,
     214,   217,   216,   218,   220,   219,   221,   222,   223,   224,
     226,   225,   228,   227,   230,   229,   231,   231,   232,   232,
     232,   232,   232,   233,   234,   235,   236,   237,   239,   238,
     240,   240,   241,   241,   243,   242,   245,   244,   246,   246,
     246,   247,   247,   249,   248,   251,   250,   253,   252,   254,
     254,   255,   255,   255,   255,   255,   255,   256,   257,   258,
     259,   260,   261,   263,   262,   264,   264,   265,   265,   267,
     266,   269,   268,   270,   270,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   271,   271,   271,   271,   271,
     271,   271,   271,   271,   271,   273,   272,   275,   274,   277,
     276,   279,   278,   281,   280,   283,   282,   285,   284,   287,
     286,   288,   288,   288,   289,   290,   292,   291,   293,   293,
     294,   294,   296,   295,   297,   297,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   298,   298,   298,   298,
     300,   299,   301,   301,   302,   302,   304,   303,   306,   305,
     307,   307,   308,   308,   309,   309,   309,   309,   309,   309,
     309,   309,   310,   311,   312,   314,   313,   316,   315,   318,
     317,   319,   321,   320,   322,   324,   323,   325,   325,   326,
     326,   328,   327,   330,   329,   331,   331,   332,   332,   333,
     333,   333,   333,   333,   333,   333,   334,   336,   335,   337,
     338,   339,   340,   342,   341,   343,   343,   344,   344,   346,
     345,   348,   347,   349,   349,   350,   350,   350,   350,   352,
     351,   354,   353,   356,   355,   357,   357,   358,   358,   360,
     359,   362,   361,   363,   363,   364,   364,   365,   365,   365,
     365,   365,   365,   365,   365,   365,   365,   365,   365,   365,
     367,   366,   369,   368,   371,   370,   373,   372,   375,   374,
     377,   376,   379,   378,   381,   380,   383,   382,   385,   384,
     387,   386,   389,   388,   391,   390,   393,   392,   394,   394,
     395,   284,   396,   396,   397,   397,   398,   398,   398,   398,
     398,   398,   398,   399,   401,   400,   402,   404,   403,   405,
     405,   406,   406,   408,   407,   410,   409,   412,   411,   414,
     413,   415,   415,   416,   416,   416,   416,   416,   416,   416,
     416,   416,   416,   416,   416,   416,   416,   416,   417,   419,
     418,   421,   420,   422,   424,   423,   425,   426,   428,   427,
     429,   429,   431,   430,   432,   433,   434,   436,   435,   437,
     437,   437,   437,   437,   439,   438,   441,   440,   443,   442,
     445,   444,   447,   446,   448,   448,   449,   451,   450,   452,
     452,   454,   453,   455,   455,   456,   456,   456,   456,   456,
     457,   459,   458,   461,   460,   462,   462,   464,   463,   465,
     465,   466,   466,   466,   466,   468,   467,   469,   470,   471
  };

  const unsigned char
  Dhcp4Parser::yyr2_[] =
  {
       0,     2,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     0,     3,     0,     3,     0,     3,
       0,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     0,     1,     3,     5,     0,
       4,     0,     1,     1,     3,     0,     4,     0,     1,     1,
       3,     2,     0,     4,     1,     3,     1,     1,     1,     1,
       1,     1,     0,     6,     0,     4,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     0,     6,     1,     3,
       1,     1,     1,     0,     4,     0,     4,     0,     4,     1,
       1,     3,     0,     6,     0,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     1,     1,     1,     1,     0,     4,     0,
       4,     0,     4,     3,     0,     4,     3,     3,     3,     3,
       0,     4,     0,     4,     0,     6,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     0,     6,
       0,     1,     1,     3,     0,     4,     0,     4,     1,     3,
       1,     1,     1,     0,     4,     0,     4,     0,     6,     1,
       3,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     0,     6,     0,     1,     1,     3,     0,
       4,     0,     4,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     1,     1,     1,     3,     3,     0,     6,     0,     1,
       1,     3,     0,     4,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     6,     0,     1,     1,     3,     0,     4,     0,     4,
       0,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1,     0,     4,     0,     4,     0,
       4,     1,     0,     4,     3,     0,     6,     0,     1,     1,
       3,     0,     4,     0,     4,     0,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     1,
       1,     3,     3,     0,     6,     0,     1,     1,     3,     0,
       4,     0,     4,     1,     3,     1,     1,     1,     1,     0,
       4,     0,     4,     0,     6,     0,     1,     1,     3,     0,
       4,     0,     4,     0,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     6,     0,     4,     0,     6,     1,     3,
       0,     4,     0,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     4,     3,     0,     6,     1,
       3,     1,     1,     0,     4,     0,     4,     0,     6,     0,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       4,     0,     4,     3,     0,     4,     3,     3,     0,     4,
       1,     1,     0,     4,     3,     3,     3,     0,     4,     1,
       1,     1,     1,     1,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     6,     1,     3,     1,     0,     6,     1,
       3,     0,     4,     1,     3,     1,     1,     1,     1,     1,
       3,     0,     4,     0,     6,     1,     3,     0,     4,     1,
       3,     1,     1,     1,     1,     0,     4,     3,     3,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Dhcp4Parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "\",\"", "\":\"", "\"[\"",
  "\"]\"", "\"{\"", "\"}\"", "\"null\"", "\"Dhcp4\"",
  "\"interfaces-config\"", "\"interfaces\"", "\"dhcp-socket-type\"",
  "\"raw\"", "\"udp\"", "\"re-detect\"", "\"echo-client-id\"",
  "\"match-client-id\"", "\"next-server\"", "\"server-hostname\"",
  "\"boot-file-name\"", "\"lease-database\"", "\"hosts-database\"",
  "\"type\"", "\"memfile\"", "\"mysql\"", "\"postgresql\"", "\"cql\"",
  "\"user\"", "\"password\"", "\"host\"", "\"port\"", "\"persist\"",
  "\"lfc-interval\"", "\"readonly\"", "\"connect-timeout\"",
  "\"contact-points\"", "\"keyspace\"", "\"valid-lifetime\"",
  "\"renew-timer\"", "\"rebind-timer\"", "\"decline-probation-period\"",
  "\"subnet4\"", "\"4o6-interface\"", "\"4o6-interface-id\"",
  "\"4o6-subnet\"", "\"option-def\"", "\"option-data\"", "\"name\"",
  "\"data\"", "\"code\"", "\"space\"", "\"csv-format\"", "\"always-send\"",
  "\"record-types\"", "\"encapsulate\"", "\"array\"",
  "\"shared-networks\"", "\"pools\"", "\"pool\"", "\"user-context\"",
  "\"subnet\"", "\"interface\"", "\"interface-id\"", "\"id\"",
  "\"rapid-commit\"", "\"reservation-mode\"", "\"disabled\"",
  "\"out-of-pool\"", "\"all\"", "\"host-reservation-identifiers\"",
  "\"client-classes\"", "\"test\"", "\"client-class\"", "\"reservations\"",
  "\"duid\"", "\"hw-address\"", "\"circuit-id\"", "\"client-id\"",
  "\"hostname\"", "\"flex-id\"", "\"relay\"", "\"ip-address\"",
  "\"hooks-libraries\"", "\"library\"", "\"parameters\"",
  "\"expired-leases-processing\"", "\"reclaim-timer-wait-time\"",
  "\"flush-reclaimed-timer-wait-time\"", "\"hold-reclaimed-time\"",
  "\"max-reclaim-leases\"", "\"max-reclaim-time\"",
  "\"unwarned-reclaim-cycles\"", "\"dhcp4o6-port\"", "\"control-socket\"",
  "\"socket-type\"", "\"socket-name\"", "\"dhcp-ddns\"",
  "\"enable-updates\"", "\"qualifying-suffix\"", "\"server-ip\"",
  "\"server-port\"", "\"sender-ip\"", "\"sender-port\"",
  "\"max-queue-size\"", "\"ncr-protocol\"", "\"ncr-format\"",
  "\"always-include-fqdn\"", "\"override-no-update\"",
  "\"override-client-update\"", "\"replace-client-name\"",
  "\"generated-prefix\"", "\"tcp\"", "\"JSON\"", "\"when-present\"",
  "\"never\"", "\"always\"", "\"when-not-present\"", "\"Logging\"",
  "\"loggers\"", "\"output_options\"", "\"output\"", "\"debuglevel\"",
  "\"severity\"", "\"flush\"", "\"maxsize\"", "\"maxver\"", "\"Dhcp6\"",
  "\"DhcpDdns\"", "\"Control-agent\"", "TOPLEVEL_JSON", "TOPLEVEL_DHCP4",
  "SUB_DHCP4", "SUB_INTERFACES4", "SUB_SUBNET4", "SUB_POOL4",
  "SUB_RESERVATION", "SUB_OPTION_DEF", "SUB_OPTION_DATA",
  "SUB_HOOKS_LIBRARY", "SUB_DHCP_DDNS", "\"constant string\"",
  "\"integer\"", "\"floating point\"", "\"boolean\"", "$accept", "start",
  "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", "$@8", "$@9", "$@10",
  "$@11", "value", "sub_json", "map2", "$@12", "map_value", "map_content",
  "not_empty_map", "list_generic", "$@13", "list_content",
  "not_empty_list", "list_strings", "$@14", "list_strings_content",
  "not_empty_list_strings", "unknown_map_entry", "syntax_map", "$@15",
  "global_objects", "global_object", "dhcp4_object", "$@16", "sub_dhcp4",
  "$@17", "global_params", "global_param", "valid_lifetime", "renew_timer",
  "rebind_timer", "decline_probation_period", "echo_client_id",
  "match_client_id", "interfaces_config", "$@18",
  "interfaces_config_params", "interfaces_config_param", "sub_interfaces4",
  "$@19", "interfaces_list", "$@20", "dhcp_socket_type", "$@21",
  "socket_type", "re_detect", "lease_database", "$@22", "hosts_database",
  "$@23", "database_map_params", "database_map_param", "database_type",
  "$@24", "db_type", "user", "$@25", "password", "$@26", "host", "$@27",
  "port", "name", "$@28", "persist", "lfc_interval", "readonly",
  "connect_timeout", "contact_points", "$@29", "keyspace", "$@30",
  "host_reservation_identifiers", "$@31",
  "host_reservation_identifiers_list", "host_reservation_identifier",
  "duid_id", "hw_address_id", "circuit_id", "client_id", "flex_id",
  "hooks_libraries", "$@32", "hooks_libraries_list",
  "not_empty_hooks_libraries_list", "hooks_library", "$@33",
  "sub_hooks_library", "$@34", "hooks_params", "hooks_param", "library",
  "$@35", "parameters", "$@36", "expired_leases_processing", "$@37",
  "expired_leases_params", "expired_leases_param",
  "reclaim_timer_wait_time", "flush_reclaimed_timer_wait_time",
  "hold_reclaimed_time", "max_reclaim_leases", "max_reclaim_time",
  "unwarned_reclaim_cycles", "subnet4_list", "$@38",
  "subnet4_list_content", "not_empty_subnet4_list", "subnet4", "$@39",
  "sub_subnet4", "$@40", "subnet4_params", "subnet4_param", "subnet",
  "$@41", "subnet_4o6_interface", "$@42", "subnet_4o6_interface_id",
  "$@43", "subnet_4o6_subnet", "$@44", "interface", "$@45", "interface_id",
  "$@46", "client_class", "$@47", "reservation_mode", "$@48", "hr_mode",
  "id", "rapid_commit", "shared_networks", "$@49",
  "shared_networks_content", "shared_networks_list", "shared_network",
  "$@50", "shared_network_params", "shared_network_param",
  "option_def_list", "$@51", "option_def_list_content",
  "not_empty_option_def_list", "option_def_entry", "$@52",
  "sub_option_def", "$@53", "option_def_params",
  "not_empty_option_def_params", "option_def_param", "option_def_name",
  "code", "option_def_code", "option_def_type", "$@54",
  "option_def_record_types", "$@55", "space", "$@56", "option_def_space",
  "option_def_encapsulate", "$@57", "option_def_array", "option_data_list",
  "$@58", "option_data_list_content", "not_empty_option_data_list",
  "option_data_entry", "$@59", "sub_option_data", "$@60",
  "option_data_params", "not_empty_option_data_params",
  "option_data_param", "option_data_name", "option_data_data", "$@61",
  "option_data_code", "option_data_space", "option_data_csv_format",
  "option_data_always_send", "pools_list", "$@62", "pools_list_content",
  "not_empty_pools_list", "pool_list_entry", "$@63", "sub_pool4", "$@64",
  "pool_params", "pool_param", "pool_entry", "$@65", "user_context",
  "$@66", "reservations", "$@67", "reservations_list",
  "not_empty_reservations_list", "reservation", "$@68", "sub_reservation",
  "$@69", "reservation_params", "not_empty_reservation_params",
  "reservation_param", "next_server", "$@70", "server_hostname", "$@71",
  "boot_file_name", "$@72", "ip_address", "$@73", "duid", "$@74",
  "hw_address", "$@75", "client_id_value", "$@76", "circuit_id_value",
  "$@77", "flex_id_value", "$@78", "hostname", "$@79",
  "reservation_client_classes", "$@80", "relay", "$@81", "relay_map",
  "$@82", "client_classes", "$@83", "client_classes_list", "$@84",
  "client_class_params", "not_empty_client_class_params",
  "client_class_param", "client_class_name", "client_class_test", "$@85",
  "dhcp4o6_port", "control_socket", "$@86", "control_socket_params",
  "control_socket_param", "control_socket_type", "$@87",
  "control_socket_name", "$@88", "dhcp_ddns", "$@89", "sub_dhcp_ddns",
  "$@90", "dhcp_ddns_params", "dhcp_ddns_param", "enable_updates",
  "qualifying_suffix", "$@91", "server_ip", "$@92", "server_port",
  "sender_ip", "$@93", "sender_port", "max_queue_size", "ncr_protocol",
  "$@94", "ncr_protocol_value", "ncr_format", "$@95",
  "always_include_fqdn", "override_no_update", "override_client_update",
  "replace_client_name", "$@96", "replace_client_name_value",
  "generated_prefix", "$@97", "dhcp6_json_object", "$@98",
  "dhcpddns_json_object", "$@99", "control_agent_json_object", "$@100",
  "logging_object", "$@101", "logging_params", "logging_param", "loggers",
  "$@102", "loggers_entries", "logger_entry", "$@103", "logger_params",
  "logger_param", "debuglevel", "severity", "$@104", "output_options_list",
  "$@105", "output_options_list_content", "output_entry", "$@106",
  "output_params_list", "output_params", "output", "$@107", "flush",
  "maxsize", "maxver", YY_NULLPTR
  };

#if PARSER4_DEBUG
  const unsigned short int
  Dhcp4Parser::yyrline_[] =
  {
       0,   228,   228,   228,   229,   229,   230,   230,   231,   231,
     232,   232,   233,   233,   234,   234,   235,   235,   236,   236,
     237,   237,   238,   238,   246,   247,   248,   249,   250,   251,
     252,   255,   260,   260,   271,   274,   275,   278,   282,   289,
     289,   296,   297,   300,   304,   311,   311,   318,   319,   322,
     326,   337,   347,   347,   360,   361,   365,   366,   367,   368,
     369,   370,   373,   373,   390,   390,   398,   399,   404,   405,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   419,   420,   421,   422,   423,   424,   425,
     428,   433,   438,   443,   448,   453,   459,   459,   469,   470,
     473,   474,   475,   478,   478,   486,   486,   496,   496,   503,
     504,   507,   513,   513,   523,   523,   533,   534,   537,   538,
     539,   540,   541,   542,   543,   544,   545,   546,   547,   548,
     549,   552,   552,   559,   560,   561,   562,   565,   565,   573,
     573,   581,   581,   589,   594,   594,   602,   607,   612,   617,
     622,   622,   630,   630,   639,   639,   649,   650,   653,   654,
     655,   656,   657,   660,   665,   670,   675,   680,   685,   685,
     695,   696,   699,   700,   703,   703,   711,   711,   719,   720,
     721,   724,   725,   728,   728,   736,   736,   744,   744,   754,
     755,   758,   759,   760,   761,   762,   763,   766,   771,   776,
     781,   786,   791,   799,   799,   812,   813,   816,   817,   824,
     824,   847,   847,   856,   857,   861,   862,   863,   864,   865,
     866,   867,   868,   869,   870,   871,   872,   873,   874,   875,
     876,   877,   878,   879,   880,   883,   883,   891,   891,   899,
     899,   907,   907,   915,   915,   923,   923,   931,   931,   939,
     939,   946,   947,   948,   951,   956,   963,   963,   974,   975,
     979,   980,   983,   983,   991,   992,   995,   996,   997,   998,
     999,  1000,  1001,  1002,  1003,  1004,  1005,  1006,  1007,  1008,
    1015,  1015,  1027,  1028,  1031,  1032,  1037,  1037,  1048,  1048,
    1058,  1059,  1062,  1063,  1066,  1067,  1068,  1069,  1070,  1071,
    1072,  1073,  1076,  1078,  1083,  1085,  1085,  1093,  1093,  1101,
    1101,  1109,  1111,  1111,  1119,  1128,  1128,  1140,  1141,  1146,
    1147,  1152,  1152,  1163,  1163,  1174,  1175,  1180,  1181,  1186,
    1187,  1188,  1189,  1190,  1191,  1192,  1195,  1197,  1197,  1205,
    1207,  1209,  1214,  1222,  1222,  1234,  1235,  1238,  1239,  1242,
    1242,  1250,  1250,  1258,  1259,  1262,  1263,  1264,  1265,  1268,
    1268,  1276,  1276,  1286,  1286,  1296,  1297,  1300,  1301,  1304,
    1304,  1312,  1312,  1320,  1321,  1324,  1325,  1329,  1330,  1331,
    1332,  1333,  1334,  1335,  1336,  1337,  1338,  1339,  1340,  1341,
    1344,  1344,  1352,  1352,  1360,  1360,  1368,  1368,  1376,  1376,
    1384,  1384,  1392,  1392,  1400,  1400,  1408,  1408,  1416,  1416,
    1424,  1424,  1437,  1437,  1447,  1447,  1458,  1458,  1468,  1469,
    1472,  1472,  1480,  1481,  1484,  1485,  1488,  1489,  1490,  1491,
    1492,  1493,  1494,  1497,  1499,  1499,  1511,  1518,  1518,  1528,
    1529,  1532,  1533,  1536,  1536,  1544,  1544,  1554,  1554,  1564,
    1564,  1572,  1573,  1576,  1577,  1578,  1579,  1580,  1581,  1582,
    1583,  1584,  1585,  1586,  1587,  1588,  1589,  1590,  1593,  1598,
    1598,  1606,  1606,  1614,  1619,  1619,  1627,  1632,  1637,  1637,
    1645,  1646,  1649,  1649,  1657,  1662,  1667,  1672,  1672,  1680,
    1683,  1686,  1689,  1692,  1698,  1698,  1708,  1708,  1715,  1715,
    1722,  1722,  1734,  1734,  1747,  1748,  1752,  1756,  1756,  1768,
    1769,  1773,  1773,  1781,  1782,  1785,  1786,  1787,  1788,  1789,
    1792,  1797,  1797,  1805,  1805,  1815,  1816,  1819,  1819,  1827,
    1828,  1831,  1832,  1833,  1834,  1837,  1837,  1845,  1850,  1855
  };

  // Print the state stack on the debug stream.
  void
  Dhcp4Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Dhcp4Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // PARSER4_DEBUG


#line 14 "dhcp4_parser.yy" // lalr1.cc:1167
} } // isc::dhcp
#line 4250 "dhcp4_parser.cc" // lalr1.cc:1167
#line 1860 "dhcp4_parser.yy" // lalr1.cc:1168


void
isc::dhcp::Dhcp4Parser::error(const location_type& loc,
                              const std::string& what)
{
    ctx.error(loc, what);
}
