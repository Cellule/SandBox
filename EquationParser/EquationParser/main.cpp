#include "Parser.h"
#include "Symbol.h"
#include <vector>
#include <map>
#include "Variable.h"
#include "Operator.h"

using namespace std;

class Program
{
public:
    Parser parser;
    void run(istream* in);

private:
    map<string, Variable*> allVariables;
};

void Program::run( istream* in )
{
    while( !in->eof() )
    {
        string buf;
        getline( *in, buf );
        if( buf.back() == '\n' )
            buf.pop_back();

        vector<Symbol*> infix, postfix, args;
        parser.parseStatement( buf, infix );
        parser.transformPostFix( infix, postfix );

        for (int i = 0; i < (int)postfix.size() ; i++)
        {
            Symbol* s = postfix[i];
            Operator* op = dynamic_cast<Operator*>(s);
            if( op )
            {

            }
            else
            {
                args.push_back( s );
            }
        }

    }
}


int main()
{
    Parser parser;
    vector<Symbol*> infix,postfix;
    parser.parseStatement( "a = (3 + 5) * 6.2", infix );
//     for (int i = 0; i < (int)infix.size() ; i++)
//     {
//         infix[i]->print( &cout );
//         cout << ' ';
//     }
//    cout << endl;
    parser.transformPostFix( infix, postfix );
//     for( int i = 0; i < (int)postfix.size(); i++ )
//     {
//         postfix[i]->print( &cout );
//         cout << ' ';
//     }


    return 0;
}