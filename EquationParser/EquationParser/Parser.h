#pragma once
#include <string>
#include <vector>
#include "Symbol.h"


class Parser
{
public:
    Parser();
    ~Parser();

    int parseStatement( const std::string& statement, std::vector<Symbol*>& symbols );
    int transformPostFix( const std::vector<Symbol*>& inSymbols, std::vector<Symbol*>& outSymbols );

};

