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

    for (int i = 0; i < tokens.size(); i++)
    {
        

        //std::cout << tokens.at(i).type << ", " << tokens.at(i).value << "\n";
        CompressedTokens compressed{};
        /*
            Variables & Functions
        */
        if (std::string(tokens.at(i).type) == "UNKNOWN")
        {
            
            int j = i-1;
            if (j <= 0 || std::string(tokens.at(i).type) == "EOL")
            {
                compressed.value = tokens.at(i).value;
                compressed.compressed.insert_or_assign("ANY", "any");
                comp_tokens.push_back(compressed);
                continue;
            }
            
            int pointers = 0;
            #define is_type(x) std::string(tokens.at(j).type) != x
            #define is_type2(x) std::string(tokens.at(j).type) == x
            while (is_type("EOL") && is_type("SEMI-COLON") && is_type("LPARENT") && is_type("COMMA"))
            {
                
                compressed.compressed.insert_or_assign(std::string(tokens.at(j).type), std::string(tokens.at(j).value));
                j--;
                if (j < 0)
                    break;
            }
            if (pointers > 0)
            {
                compressed.compressed.insert_or_assign("POINTER", std::to_string(pointers));
                
                int j = comp_tokens.size() - pointers;

                //comp_tokens.(j, j+pointers);

            }
            compressed.value = tokens.at(i).value;
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
                        compressed.value = "NUMBER";
                        compressed.compressed.insert_or_assign("NUMBER", (tokens.at(i).value + tokens.at(i+1).value + tokens.at(i+2).value));
                        i+=2;
                    }
                    else
                    {
                        std::cerr << "PARSING COMPRESSION ERROR: \nFloat|Double needs some digits after a point.\n";
                    }
                }
                else
                {
                    std::cerr << "PARSING COMPRESSION ERROR: \nReached EOF after a period.\n";
                }
            }
            else
            {
                compressed.value = "INT";
                compressed.compressed.insert_or_assign("INT", (tokens.at(i).value));
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
            compressed.compressed.insert_or_assign(tokens.at(i).type, tokens.at(i).value);
            compressed.value = tokens.at(i).value;
        }
        comp_tokens.push_back(compressed);
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
        std::cout<< comp_tokens.at(i).value << "\n";
        for (auto const& [k, v] : comp_tokens.at(i).compressed)
        {
            std::cout << k << "\t\t" << v << "\n";
        }
        std::cout << "***\n";
    }
}

void Parser::parse()
{
    Parser::display_compressed();
}

/* 
variable + equals + number + eol 
*/