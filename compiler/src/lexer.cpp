#include "lexer.h"


Lexer::Lexer()
{

}

// Suky
void Lexer::add_token(const char* type, const char* detected)
{
    comp_tokens.insert_or_assign(std::string(detected), type);
    
}

void Lexer::add_delimiter(const char* type, char delimiter)
{
    delimiters.insert_or_assign(delimiter, type);
}

// Suky
const char* Lexer::lowerCase(const char* value)
{
    std::string buf = "";
    for (int i = 0; i < sizeof(value); i++)
        buf += tolower(value[i]);
    return buf.c_str();
}

bool Lexer::is_number(char* number)
{
    std::string buffer = number;

    bool correct = true;
    for (char c : buffer)
    {

        if (!std::isdigit(c))
        {
            correct = false;
        }
    }
    return correct;
}

// Loner

bool Lexer::is_delimiter(char c)
{
    return delimiters.count(c);
}

void Lexer::print_tokens()
{
    for (auto token : tokens)
    {
        std::cout << "type " << token.type << "   value " << token.value << "\n";
    }
}

Lexer::~Lexer()
{
    tokens.clear();
    delimiters.clear();
    comp_tokens.clear();
}

void Lexer::token_type(char* token)
{

    if (comp_tokens.count(std::string(token)))
    {
        
        // keyword exist
        const char* tokenType = comp_tokens.at(std::string(token));
        //std::cout << "type " << tokenType << "   value " << token << "\n";

        Token _token {tokenType, token};
        tokens.push_back(_token);

        //free(&tokenType);
        //free(&_token);
    }
    else if (delimiters.count(token[0]) && sizeof(token) == 8)
    {
        
        // Is a delimiter

        const char* delimiterType = delimiters.at(token[0]);
        //if (token[0] == ' ')
        //    std::cout << "Yay!\n";
        //std::cout << "type " << delimiterType << "   value " << token << "\n";
        Token _token {delimiterType, token};
        tokens.push_back(_token);

        //free(&delimiterType);
        //free(&_token);
    } 
    else
    {
        // We ignore this
        if (Lexer::is_number(token))
        {
            Token _token {"DIGITS", token};
            tokens.push_back(_token);

            //free(&_token);
        }
        else
        {
            Token _token {"UNKNOWN", token};
            tokens.push_back(_token);

            //free(&_token);
        }
    }
}


void Lexer::read_line(const char* line){
    char token[256] = "";
    int index = 0;
    bool istring = false;
    bool comment = false;
    for(int i = 0; line[i] != '\0'; i++){
        if(Lexer::is_delimiter(line[i])){
            if(!comment){
                if(token[0]!='\0'){
                    token[index] = '\0';
                    index = 0;
                    // if(line[i]!='"' && istring){
                        Lexer::token_type(token);
                    // }
                }
                token[0] = line[i];
                token[1] = '\0';
                if (line[i] != ' '){
                    Lexer::token_type(token);
                }
                token[0] = '\0';
                if(line[i]=='#')
                    comment = true;
                if(line[i]=='"')
                    istring = !istring;
            }
        } else {
            if(index<256)
                token[index++] = line[i];
        }
    } 
}

