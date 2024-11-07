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
    Parser(std::vector<Token> tokens);
    bool in_blacklist(int ind=0);
    void compress(std::vector<Token> tokens);
    void display_compressed();
    void add_to_blacklist(std::string token);
    void advance(int a = 1);
    bool end();
    Token get(int a = 0);
    void storeToken(CompressedTokens token);
    void storeToken(std::string type, std::map<std::string, std::string> value);
    void pushType(std::string type);
    void pushValue(std::string key, std::string value);
    void clearToken();
    void storeToken();
    std::string compile();
    std::vector<CompressedTokens> comp_tokens{};
    std::vector<std::string> blacklist{};
    private:
    CompressedTokens current{};
    int index = 0;
    std::vector<Token> tokens{};
};