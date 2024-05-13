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
    /*for (int i = 0; i < tokens.size(); i++)
    {
        std::cout << tokens.at(i).type << "\n";
    }*/

    int line = 1;

    for (int i = 0; i < tokens.size(); i++)
    {
        

        //std::cout << tokens.at(i).type << ", " << tokens.at(i).value << "\n";
        CompressedTokens compressed{};
        /*
            Variables & Functions
        */

        if (std::string(tokens.at(i).type) == "EOL")
        {
            line++;
        }

        if (std::string(tokens.at(i).type) == "QUOTE")
        {
            
            int j = i+1;
            std::string buffer = "";
            if (j >= tokens.size())
            {
                std::cerr << "Missed an \n";
                exit(1);
            }
            while (std::string(tokens.at(j).type) != "QUOTE")
            {
                buffer += tokens.at(j).value;
                j++;
                if (j >= tokens.size())
                {
                    std::cerr << "Line " << line << " Missed an \"\n";
                    exit(1);
                }
            }
            i = j;
            compressed.type = "string";
            compressed.value.insert_or_assign("value", buffer);
            comp_tokens.push_back(compressed);
        }

        if (std::string(tokens.at(i).type) == "MINUS")
        {
            if (i+2 >= tokens.size())
            {
                continue;
            }
            if (std::string(tokens.at(i+1).type) == "GREATER")
            {
                compressed.type = "funcType";
                compressed.value.insert_or_assign("value", std::string(tokens.at(i+2).value));
                comp_tokens.push_back(compressed);
                i+=2;
            }
        }
        else if (std::string(tokens.at(i).type) == "IMPORT")
        {
            int j = i+1;
            std::string buffer = "";
            if (j >= tokens.size())
            {
                std::cerr << "Missed an \n";
                exit(1);
            }
            while (std::string(tokens.at(j).type) != "EOL")
            {
                buffer += tokens.at(j).value;
                j++;
                if (j >= tokens.size())
                {
                    std::cerr << "Line " << line << " import error?\n";
                    exit(1);
                }
            }
            i = j;
            compressed.type = "include";
            compressed.value.insert_or_assign("value", buffer);
            comp_tokens.push_back(compressed);
        }
        
        else if (std::string(tokens.at(i).type) == "UNKNOWN")
        {

            // Checking for the right
            

            // Checking for the left
            
            int j = i-1;
            if (j <= 0 || std::string(tokens.at(i-1).type) == "EOL")
            {
                int k = i+1;

                if (k < tokens.size())
                {
                    //std::cout << tokens.at(k).type << "\n";
                    if (std::string(tokens.at(k).type) == "LPARENT")
                    {
                        //std::cout << "Test!\n";
                        compressed.type = "funcCall";
                        compressed.value.insert_or_assign("value", tokens.at(i).value);
                        comp_tokens.push_back(compressed);
                        i++;
                        continue;
                    }
                }
                compressed.type = "declaration";
                compressed.value.insert_or_assign("value", tokens.at(i).value);
                compressed.value.insert_or_assign("ANY", "any");
                comp_tokens.push_back(compressed);
                continue;
            }
            
            int pointers = 0;
            #define is_type(x) std::string(tokens.at(j).type) != x
            #define is_type2(x) std::string(tokens.at(j).type) == x
            compressed.value.insert_or_assign("value", tokens.at(i).value);
            while (is_type("EOL") && is_type("SEMI-COLON") && is_type("LPARENT") && is_type("COMMA"))
            {
                compressed.value.insert_or_assign(std::string(tokens.at(j).type), std::string(tokens.at(j).value));
                j--;
                if (j < 0)
                    break;
            }
            if (pointers > 0)
            {
                compressed.value.insert_or_assign("POINTER", std::to_string(pointers));
                
                int j = comp_tokens.size() - pointers;

                //comp_tokens.(j, j+pointers);

            }
            compressed.type = "declaration";
            comp_tokens.push_back(compressed);
        }
        /*
            Floats, Doubles & Integer
        */
        else if (std::string(tokens.at(i).type) == "DIGITS")
        {
            bool _float = 0;
            if (i + 1 < tokens.size())
            {
                if (std::string(tokens.at(i+1).type) == "PERIOD")
                    _float = 1;
            }
            if (_float)
            {
                if (i + 2 < tokens.size())
                {
                    if (std::string(tokens.at(i+2).type) == "DIGITS")
                    {
                        compressed.type = "number";
                        compressed.value.insert_or_assign("value", (tokens.at(i).value + tokens.at(i+1).value + tokens.at(i+2).value));
                        comp_tokens.push_back(compressed);
                        i+=2;
                    }
                    else
                    {
                        std::cerr << "Line " << line << " PARSING COMPRESSION ERROR: \nFloat|Double needs some digits after a point.\n";
                        exit(1);
                    }
                }
                else
                {
                    std::cerr << "Line " << line << " PARSING COMPRESSION ERROR: \nReached EOF after a period.\n";
                    exit(1);
                }
            }
            else
            {
                compressed.type = "int";
                compressed.value.insert_or_assign("value", (tokens.at(i).value));
                comp_tokens.push_back(compressed);
            }
        }
        else
        {
            if (i + 1 < tokens.size())
            {
                if (std::string(tokens.at(i+1).type) == "UNKNOWN")
                    continue;
            }
            bool found = false;
            for (auto d : blacklist)
            {
                if (d == std::string(tokens.at(i).type))
                    found = true;
            }
            if (found)
                continue;
            compressed.value.insert_or_assign(tokens.at(i).type, tokens.at(i).value);
            compressed.type = tokens.at(i).type;
            comp_tokens.push_back(compressed);
        }
        //comp_tokens.push_back(compressed);
    }

    
}

void Parser::add_to_blacklist(std::string token)
{
    blacklist.push_back(token);
}

void Parser::display_compressed()
{
    for(int i = 0; i < comp_tokens.size();i++)
    {
        std::cout << i << "\t" << comp_tokens.at(i).type << "\n";
        for (auto const& [k, v] : comp_tokens.at(i).value)
        {
            std::cout << k << "\t\t" << v << "\n";
        }
    }
}

std::string Parser::compile()
{

    std::string buffer = "";
    

    for (int i =0; i < comp_tokens.size(); i++)
    {
        auto token = comp_tokens.at(i);
        if (token.type == "include")
        {
            buffer += "#include <";
            buffer += token.value.at("value");
            buffer += ">";
        }
        else if (token.type == "declaration")
        {
            if (token.value.count("FUNCTION"))
            {
                // This is a function declaration
                std::string name = token.value.at("value");
                std::string type = "";
                std::string args = "";
                i+=2;
                while (comp_tokens.at(i).type != "RPARENT")
                {
                    // arguments
                    i++;
                }
                i++; // skip
                if (comp_tokens.at(i).type == "funcType")
                {
                    type = comp_tokens.at(i).value.at("value");
                }
                buffer += type;
                buffer += " ";
                buffer += name;
                buffer += "(";
                buffer += args;
                buffer += ")";
            }
            else
            {
                // Variable ?
            }
        }
        else if (token.type  == "RETURN")
        {
            buffer += "return ";
            buffer += comp_tokens.at(i+1).value.at("value");
            buffer += ";";
        }
        else if (token.type == "funcCall")
        {
            buffer += token.value.at("value");
            buffer += "(";
            i++;
            while (comp_tokens.at(i).type != "RPARENT")
            {
                if (comp_tokens.at(i).type == "string")
                    buffer += "\"";
                buffer += comp_tokens.at(i).value.at("value");
                if (comp_tokens.at(i).type == "string")
                    buffer += "\"";
                i++;
            }
            buffer += ");";
        }
        else if (token.type == "LCURLYBR")
        {
            buffer += "{";
        }
        else if (token.type == "RCURLYBR")
        {
            buffer += "}";
        }
        else if (token.type == "EOL")
        {
            buffer += "\n";
        }
    }

    //buffer += "int main(){\n";

    
    
    //buffer += "return 0;\n}";

    return buffer;
}
