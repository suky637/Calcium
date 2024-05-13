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
    std::map<std::string, std::string> value;
    std::string type;
};

class Parser
{
    public:

    void compress(std::vector<Token> tokens);
    void display_compressed();
    void add_to_blacklist(std::string token);
    std::string compile();
    private:
    std::vector<CompressedTokens> comp_tokens{};
    std::vector<std::string> blacklist{};
};