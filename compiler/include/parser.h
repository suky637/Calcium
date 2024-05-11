#pragma once
/*
    STEP 1

    compressing the variable initializers

    local lock int x = 10;

    {{"local", "lock", "int"}, UNKNOWN, EQUALS, DIGITS}

    100.103
    {NUMBER[100.103]}

    default: float

    FIRST PART


*/

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "lexer.h"

struct CompressedTokens
{
    std::map<std::string, std::string> compressed;
};

class Parser
{
    public:

    void compress(std::vector<Token> tokens);
    void parse();
    private:
    std::vector<CompressedTokens> comp_tokens{};

};