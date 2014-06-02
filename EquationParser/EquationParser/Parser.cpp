#include "Parser.h"
#include <map>
#include "Variable.h"
#include "Operator.h"
#include <regex>
#include "ConstantInt.h"
#include "ConstantFloat.h"


const int lookAhead = 3;
using namespace std;

map<string, OperatorTokens> tkMapping;

Parser::Parser()
{
    if( tkMapping.empty() )
    {
#define TOK_DCL(tk,pre,sym,...) tkMapping[sym] = tk;
#include "OperatorList.h"
#undef TOK_DCL        
    }
}


Parser::~Parser()
{
}

Symbol* convertStr( const string& str )
{
    static regex rxInt( "-?([1-9][0-9]*)|0" );
    static regex rxFloat( "-?([0-9]+)[\\.,]([0-9]+)" );

    Symbol* sym = 0;

    cmatch res;
    if( regex_search( str.c_str(), res, rxFloat ) )
    {
        int val = atoi( res[1].str().c_str() );
        double fraction = atoi( res[2].str().c_str() );
        while( fraction > 1 )fraction /= 10;
        fraction += val;
        if( str[0] == '-' )
        {
            fraction *= -1;
        }
        ConstantFloat* ci = new ConstantFloat();
        ci->mValue = fraction;
        sym = ci;
    }
    else if( regex_match( str.begin(), str.end(), rxInt ) )
    {
        int value = atoi( str.c_str() );
        ConstantInt* ci = new ConstantInt();
        ci->mValue = value;
        sym = ci;
    }
    else
    {
        Variable* v = new Variable();
        v->mName = str;
        sym = v;
    }

    return sym;
}


int Parser::parseStatement( const std::string& statement, std::vector<Symbol*>& symbols )
{
    string buffer;
    const int stmtLength = statement.length();
    int i = 0;
    int processEnd = 0;
    while( i < stmtLength || !buffer.empty() )
    {
        while( i < stmtLength && statement[i] == ' ' ) ++i;
        if( i < stmtLength )
            buffer.push_back( statement[i++] );

        while( buffer.length() < lookAhead + 1 && i < stmtLength )
        {
            if( statement[i] != ' ' )
            {
                buffer.push_back( statement[i] );
            }
            ++i;
        }

        const int bufSiz = buffer.length();
        const int lookAtSize = min( bufSiz, lookAhead + 1 - processEnd );
        if( lookAtSize == 0 )
        {
            symbols.push_back( convertStr( buffer ) );
            break;
        }
        {
            string tmp;
            tmp.resize( lookAtSize );
            memcpy( (char*)tmp.c_str(), buffer.c_str() + ( bufSiz - lookAtSize ), lookAtSize );

            while( !tmp.empty() )
            {
                const auto it = tkMapping.find( tmp );
                if( it != tkMapping.cend() )
                {
                    if( bufSiz > lookAtSize )
                    {
                        // contains symbols before token
                        string val = buffer.substr( 0, bufSiz - lookAtSize );
                        symbols.push_back( convertStr(val) );
                    }
                    Operator* op = new Operator();
                    op->mOp = it->second;
                    symbols.push_back( op );

                    const int bufLeft = lookAtSize - tmp.size();
                    buffer = buffer.substr( bufSiz - bufLeft, bufLeft );
                    break;
                }
                tmp.pop_back();
            }
        }
        if( i >= stmtLength )
        {
            processEnd++;
        }
    }


    return 1;
}

void popOp( std::vector<Symbol*>& outSymbols, std::vector<Operator*>& stack )
{
    if( Operator::opDefs[stack.back()->mOp].nbOperand > 0 )
    {
        outSymbols.push_back( stack.back() );
    }
    else
    {
        delete stack.back();
    }
    stack.pop_back();
}


int Parser::transformPostFix( const std::vector<Symbol*>& inSymbols, std::vector<Symbol*>& outSymbols )
{
    vector<Operator*> stack;
    for (int i = 0; i < (int)inSymbols.size() ; i++)
    {
        Symbol* s = inSymbols[i];
        Operator* op = dynamic_cast<Operator*>( s );
        if( op )
        {
            const Precedence pre = Operator::opDefs[op->mOp].pre;
            while( !stack.empty() && stack.back()->mOp != tkLParen && stack.back()->mOp != tkLBrack && Operator::opDefs[stack.back()->mOp].pre >= pre )
            {
                popOp( outSymbols, stack );
            }
            if( op->mOp == tkRParen )
            {
                while( !stack.empty() && stack.back()->mOp != tkLParen )
                {
                    popOp( outSymbols, stack );
                }
                if( stack.empty() )
                {
                    // parenthesis error
                    return false;
                }
                delete stack.back();
                delete op;
                stack.pop_back();
            }
            else if( op->mOp == tkRBrack )
            {
                while( !stack.empty() && stack.back()->mOp != tkLBrack )
                {
                    popOp( outSymbols, stack );
                }
                if( stack.empty() )
                {
                    // parenthesis error
                    return false;
                }
                delete stack.back();
                delete op;
                stack.pop_back();
            }
            else
            {
                stack.push_back( op );
            }
        }
        else
        {
            outSymbols.push_back( s );
        }
    }
    while( !stack.empty() )
    {
        popOp( outSymbols, stack );
    }

    return 1;
}
