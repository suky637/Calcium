#include "parser.h"

/*
    STEP 1

    compressing the variable initializers

    local lock int x = 10;

    {{"local", "lock", "int"}, UNKNOWN, EQUALS, DIGITS}

    100.103
    {NUMBER[100.103]}

    default: float

    
    STEP 2

    
    [
        "var": [
            "local",
            "lock",
            "int"
        ],
        "varName": "x",
        "value": "10"
    ]
    ^--- local lock int x = 10;

    [
        "function": "void",
        "functionName": "test",
        "arguments": {
            "x": "string",
            "y": "int"
        }
    ]
    ^--- fn test(string x, int y)
    {
        
    }
*/

void Parser::compress(std::vector<Token> tokens)
{
    for (int i = 0; i < tokens.size(); i++)
    {
        CompressedTokens compressed{};
        compressed.compressed.insert_or_assign(tokens.at(i).type, tokens.at(i).value);
    }
}

void Parser::parse()
{
    for(int i = 0; i < comp_tokens.size();i++)
    {
        std::cout << '\0';    
    }
}

/* 
variable + equals + number + eol 




*/