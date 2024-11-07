#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "compiler_types.h"

struct Token
{
    std::string type;
    std::string value;
};

class Lexer
{
    public:
    Lexer();
    ~Lexer();
    // Suky
    // Adding a keyword to the program
    void add_token(const char* type, const char* detected);

    // Suky
    // Adding a delimiters
    void add_delimiter(const char* type, char delimiter);

    // Suky
    // Lower case the desired C string
    const char* lowerCase(const char* value);

    bool is_number(std::string number);

    void token_type(std::string token, int type = 0);

    void print_tokens();

    // Loner
    // is_delimiter
    bool is_delimiter(char character);

    // Loner
    // Tokenize a text
    void read_line(const char* line);

    std::vector<Token> tokens{};
    std::map<char, const char*> delimiters{};
    std::map<std::string, const char*> comp_tokens{};
    private:
    // Key: type
    // Value: What will be detected by the script

};

