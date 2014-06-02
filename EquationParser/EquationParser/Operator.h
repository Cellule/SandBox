#pragma once
#include "Symbol.h"

enum Precedence
{
    Cma
    , Asg
    , Que
    , Lor
    , Lan
    , Bor
    , Xor
    , Ban
    , Equ
    , Cmp
    , Shf
    , Add
    , Mul
    , No
};

enum OperatorTokens
{
#define TOK_DCL(name,...) name,
#include "OperatorList.h"
#undef TOK_DCL
    tkMaxOperator,
};

struct OperatorDef
{
    OperatorDef( OperatorTokens _tk, Precedence _pre, const std::string& _val, int _nbOperand ) :tk( _tk ), pre( _pre ), val( _val ), nbOperand( _nbOperand )
    {
    }
    OperatorTokens tk;
    Precedence pre;
    std::string val;
    int nbOperand;
};

class OperatorArgs
{
public:
    OperatorArgs( int _size ) :argsIn( 0 ), size( _size )
    {
        args = new Symbol*[size];
    }
    ~OperatorArgs()
    {
        delete[] args;
    }
    void addArg( Symbol* s )
    {
        if( argsIn < size )
        {
            args[argsIn++] = s;
        }
    }

    Symbol** args;
    const int size;
    int argsIn;
};

typedef void( *OperatorOperation )( const OperatorArgs&);

class Operator :
    public Symbol
{
public:
    Operator();
    ~Operator();

    virtual void print( std::ostream* out ) const;

    OperatorTokens mOp;

    static const OperatorDef opDefs[tkMaxOperator];

// operator implementation
public:
    static inline void Add_A( const OperatorArgs& arg )
    {
        int left = 0;
    }
    /*
TOK_DCL( tkDArrow   , Asg, "=>"  ,2)
TOK_DCL( tkAsg      , Asg, "="   ,2)
TOK_DCL( tkAsgAdd   , Asg, "+="  ,2)
TOK_DCL( tkAsgSub   , Asg, "-="  ,2)
TOK_DCL( tkAsgMul   , Asg, "*="  ,2)
TOK_DCL( tkAsgDiv   , Asg, "/="  ,2)
TOK_DCL( tkAsgMod   , Asg, "%="  ,2)
TOK_DCL( tkAsgAnd   , Asg, "&="  ,2)
TOK_DCL( tkAsgXor   , Asg, "^="  ,2)
TOK_DCL( tkAsgOr    , Asg, "|="  ,2)
TOK_DCL( tkAsgLsh   , Asg, "<<=" ,2)
TOK_DCL( tkAsgRsh   , Asg, ">>=" ,2)
TOK_DCL( tkAsgRs2   , Asg, ">>>=",2)
TOK_DCL( tkQMark    , Que, "?"   ,3)
TOK_DCL( tkColon    , No , ":"   ,2)
TOK_DCL( tkLogOr    , Lor, "||"  ,2)
TOK_DCL( tkLogAnd   , Lan, "&&"  ,2)
TOK_DCL( tkOr       , Bor, "|"   ,2)
TOK_DCL( tkXor      , Xor, "^"   ,2)
TOK_DCL( tkAnd      , Ban, "&"   ,2)
TOK_DCL( tkEQ       , Equ, "=="  ,2)
TOK_DCL( tkNE       , Equ, "!="  ,2)
TOK_DCL( tkEqv      , Equ, "===" ,2)
TOK_DCL( tkNEqv     , Equ, "!==" ,2)
TOK_DCL( tkLT       , Cmp, "<"   ,2)
TOK_DCL( tkLE       , Cmp, "<="  ,2)
TOK_DCL( tkGT       , Cmp, ">"   ,2)
TOK_DCL( tkGE       , Cmp, ">="  ,2)
TOK_DCL( tkLsh      , Shf, "<<"  ,2)
TOK_DCL( tkRsh      , Shf, ">>"  ,2)
TOK_DCL( tkRs2      , Shf, ">>>" ,2)
TOK_DCL( tkAdd      , Add, "+"   ,2)
TOK_DCL( tkSub      , Add, "-"   ,2)
TOK_DCL( tkStar     , Mul, "*"   ,2)
TOK_DCL( tkDiv      , Mul, "/"   ,2)
TOK_DCL( tkPct      , Mul, "%"   ,2)
TOK_DCL( tkTilde    , No , "~"   ,1)
TOK_DCL( tkBang     , No , "!"   ,1)
TOK_DCL( tkInc      , No , "++"  ,1)
TOK_DCL( tkDec      , No , "--"  ,1)
*/
};

