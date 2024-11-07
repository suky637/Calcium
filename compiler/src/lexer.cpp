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

bool Lexer::is_number(std::string number)
{

    bool correct = true;
    for (char c : number)
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
        std::cout << "type " << token.type << "   value #" << token.value << "#\n";
    }
}

Lexer::~Lexer()
{
    tokens.clear();
    delimiters.clear();
    comp_tokens.clear();
}

void Lexer::token_type(std::string token, int type)
{
    if (type == 1)
    {
        Token _token {"STRING", token};
        tokens.push_back(_token);
        return;
    }

    if (comp_tokens.count(std::string(token)))
    {
        
        // keyword exist
        const char* tokenType = comp_tokens.at(token);
        //std::cout << "type " << tokenType << "   value " << token << "\n";

        Token _token {tokenType, token};
        tokens.push_back(_token);

        //free(&tokenType);
        //free(&_token);
    }
    else if (delimiters.count(token[0]) && token.size() == 1)
    {
        
        // Is a delimiter
        //std::cout << "TEST?\n";

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


void Lexer::read_line(const char* line)
{
    std::string token = "";
    static std::string currentString = "";
    static bool instring = false;
    static bool comment = false;
    for (auto c : std::string(line))
    {
        token += c;
        if (!instring && c == '#')
        {
            Token _token{};
            _token.type = delimiters.at('\n');
            _token.value = '\n';
            tokens.push_back(_token);
            break;
        }
        if (Lexer::is_delimiter(c))
        {
            if (c == '"')
            {
                instring = !instring;
                token = "";
                if (instring == false)
                {
                    Token _token{};
                    _token.type = "STR";
                    _token.value = currentString;
                    tokens.push_back(_token);
                    currentString = "";
                }
            }
            else
            {
                if (comp_tokens.count(token.substr(0, token.size()-1)))
                {
                    Token _token{};
                    _token.type = comp_tokens.at(token.substr(0, token.size()-1));
                    _token.value = token.substr(0, token.size()-1);
                    tokens.push_back(_token);
                    token = "";
                }
                else 
                {
                    if (token.substr(0, token.size()-1) != "")
                    {
                        if (is_number(token.substr(0, token.size()-1)))
                        {
                            Token _token{};
                            _token.type = "DIGIT";
                            _token.value = token.substr(0, token.size()-1);
                            tokens.push_back(_token);
                            token = "";

                        }
                        else
                        {
                            Token _token{};
                            _token.type = "UNKNOWN";
                            _token.value = token.substr(0, token.size()-1);
                            tokens.push_back(_token);
                            token = "";
                        }
                    }
                }

                if (!instring)
                {
                    // put delimiters
                    if (c == ' ' || c == '\t') { token = ""; continue; }
                    Token _token{};
                    _token.type = delimiters.at(c);
                    _token.value = c;
                    tokens.push_back(_token);
                    token = "";
                }
            }
        }
        if (instring)
        {
            if (c == '"') continue;
            currentString+=c;
            token = "";
        }
    }
}

/*int is_string = 0;
int test = 1;
void Lexer::read_line(const char* line){
    std::string token = "";
    static std::string currentString = "";
    static bool instring = false;
    token.resize(1);
    int index = 0;
    int comment = 0;
    //std::cout << token.size() << "\n";
    for(int i = 0; line[i] != '\0'; i++){
        //std::cout << is_delimiter(line[i]) << '\n';
        
        if(Lexer::is_delimiter(line[i]) && (!instring || (line[i]=='"' && instring)) || line[i]==';'){
            if(!comment){
                if(token[0]!='\0'){
                    token[index] = '\0';
                    index = 0;
                    if(line[i] == '"')
                    {
                        Lexer::token_type(token, 1);
                        instring = false;
                    }
                    else
                        Lexer::token_type(token);
                } 
                token[0] = line[i];
                token.resize(1);
                if(line[i]!=' ')
                    Lexer::token_type(token); 
                token.resize(1);
                token.clear();
                if(line[i]=='#')
                    comment = 1;
                if(line[i]=='"' && !instring)
                    instring = true;
            }
        } else {
            if(line[i]!='\t'){
                if (index >= token.size()){
                    token.resize(token.size()+1);
                    //std::cout << token.size() << "\n";
                }
                
                token[index++] = line[i];
            }

        }
    }    
}*/
// void Lexer::read_line(const char* line){
//     char token[256] = "";
//     int index = 0;
//     bool istring = false;
//     bool comment = false;
//     for(int i = 0; line[i] != '\0'; i++){
//         if(Lexer::is_delimiter(line[i])){
//             if(!comment){
//                 if(token[0]!='\0'){
//                     token[index] = '\0';
//                     index = 0;
//                     // if(line[i]!='"' && istring){
//                         Lexer::token_type(token);
//                     // }
//                 }
//                 token[0] = line[i];
//                 token[1] = '\0';
//                 if (line[i] != ' '){
//                     Lexer::token_type(token);
//                 }
//                 token[0] = '\0';
//                 if(line[i]=='#')
//                     comment = true;
//                 if(line[i]=='"')
//                     istring = !istring;
//             }
//         } else {
//             if(index<256)
//                 token[index++] = line[i];
//         }
//     } 
// }

