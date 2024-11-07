#include <iostream>
#include <fstream>
#include <lexer.h>
#include <parser.h>

/*
    Suky_Laplante

    CALCIUM PROGRAMMING LANGUAGE

    FILE EXTENSION: *.cal

    VERSION: 1.0-InDev
*/

int main(int argc, char* argv[])
{
    
    // Make code fast
    //std::ios::sync_with_stdio(false);
    if (argc != 2){
        std::cout << "Usage: " << argv[0] << " <filename>" << "\n";
        return 1;
    }

    Lexer lexer{};
    
    // reserved delimeters
    lexer.add_delimiter("EOL", '\n');
    lexer.add_delimiter("SPACE", ' ');
    lexer.add_delimiter("TAB", '\t');
    lexer.add_delimiter("SEMI-COLON", ';');
    lexer.add_delimiter("COMMA", ',');
    lexer.add_delimiter("PERIOD", '.');
    lexer.add_delimiter("EQUALS", '=');
    lexer.add_delimiter("LOWER", '<');
    lexer.add_delimiter("GREATER", '>');
    lexer.add_delimiter("MINUS", '-');
    lexer.add_delimiter("PLUS", '+');
    lexer.add_delimiter("MULT", '*');
    lexer.add_delimiter("DIVIDE", '/');
    lexer.add_delimiter("COMMENTS", '#');
    lexer.add_delimiter("MODULO", '%');
    lexer.add_delimiter("LPARENT", '(');
    lexer.add_delimiter("RPARENT", ')');
    lexer.add_delimiter("LCURLYBR", '{');
    lexer.add_delimiter("RCURLYBR", '}');
    lexer.add_delimiter("SMOL_QUOTE", '\'');
    lexer.add_delimiter("LBRAQ", '[');
    lexer.add_delimiter("RBRAQ", ']');
    lexer.add_delimiter("QUOTE", '"');
    lexer.add_delimiter("NULL", '?');

    // reserved words
    lexer.add_token("CONST", "const");
    //lexer.add_token("LOCK", "lock");
    lexer.add_token("INT", "int");
    //lexer.add_token("STRING", "string");
    lexer.add_token("CHAR", "char");
    //lexer.add_token("ANY", "any");
    lexer.add_token("RETURN", "return");
    lexer.add_token("BOOLEAN", "bool");
    lexer.add_token("FLOAT", "float");
    lexer.add_token("DOUBLE", "double");
    //lexer.add_token("STRING", "string");
    //lexer.add_token("LOCAL", "local");
    //lexer.add_token("MATH_FUNC", "func"); // TODO: LATER
    lexer.add_token("FUNCTION", "fn");
    lexer.add_token("RETURN", "return");
    lexer.add_token("BOOLEAN", "bool");
    lexer.add_token("FLOAT", "float");
    lexer.add_token("DOUBLE", "double");
    lexer.add_token("IMPORT", "import");
    lexer.add_token("POINTER", "point");
    lexer.add_token("REFERENCE", "ref");
    lexer.add_token("IF", "if");
    lexer.add_token("THEN", "then");
    lexer.add_token("AND", "and");
    lexer.add_token("OR", "or");
    lexer.add_token("DATA", "data");
    lexer.add_token("NAMESPACE", "namespace");
    lexer.add_token("ENDNAMESPACE", "endnamespace");
    /////////////////////////////////////////////////////////////////////
    // PROCESSING THE LANGUAGE
    /////////////////////////////////////////////////////////////////////
    
    std::ifstream file(argv[1]);
    std::string line;

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }
    while (std::getline(file, line))
    {
        lexer.read_line((line + '\n').c_str());
    }

    file.close();

    //lexer.print_tokens();
    
    Parser parser{lexer.tokens};

    parser.add_to_blacklist("SPACE");
    parser.add_to_blacklist("TAB");

    
    //parser.compress(lexer.tokens);

    std::string currentNamespace = "";

    while (!parser.end())
    {
        
        if (parser.get().type == "IMPORT")
        {
            parser.pushType("INCLUDE");
            parser.pushValue("value", parser.get(1).value);
            parser.advance();
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "NAMESPACE")
        {
            currentNamespace = parser.get(1).value + "__";
            parser.advance();
        }
        else if (parser.get().type == "ENDNAMESPACE")
        {
            currentNamespace = "";
        }
        else if (parser.get().type == "UNKNOWN")
        {
            if (parser.get(1).type == "EQUALS" || parser.get(1).type == "NULL")
            {
                parser.pushType("Declaration");
                parser.pushValue("value", parser.get().value);
                int ind = -1;
                std::string args = "";
                while (true)
                {
                    if (parser.in_blacklist(ind))
                    {
                        ind--;
                        continue;
                    }
                    if (parser.get(ind).type == "EOL")
                        break;
                    if (parser.get(ind).type == "Error")
                        break;
                    if (parser.get(ind).type == "SEMI-COLON")
                        break;
                    if (parser.get(ind).type == "LPARENT")
                        break;
                    if (parser.get(ind).type == "COMMA")
                        break;
                    if (parser.get(ind).type == "POINTER")
                        args += "*";
                    else
                        args += parser.get(ind).value + " ";

                    //parser.pushValue(parser.get(ind).type, parser.get(ind).value);
                    ind--;
                }
                parser.pushValue("TYPE", args);
                parser.storeToken();
                parser.clearToken();
            }
            else if (parser.get(1).type == "LPARENT")
            {
                if (parser.get(-1).type == "FUNCTION")
                {
                    parser.pushType("Function");
                    parser.pushValue("value", currentNamespace + parser.get().value);
                    parser.storeToken();
                    parser.clearToken();
                }
                else
                {
                    parser.pushType("Call");
                    parser.pushValue("value", parser.get().value);
                    parser.advance(2);
                    std::string args = "";
                    while (parser.get().type != "RPARENT")
                    {
                        if (parser.get().type == "STR")
                            args += "\"" + parser.get().value + "\",";
                        else if (parser.get().type == "POINTER")
                            args += "*";
                        else if (parser.get().type == "REFERENCE")
                            args += "&";
                        else if (parser.get().type == "DIGIT" || 
                        parser.get().type == "UNKNOWN")
                            args += parser.get().value + ",";
                        parser.advance();
                    }
                    args = args.substr(0, args.size()-1);
                    parser.pushValue("args", args);
                    parser.advance();
                    parser.storeToken();
                    parser.clearToken();
                }
            }
        }
        else if (parser.get().type == "DATA")
        {
            parser.pushType("DATA");
            if (parser.get(1).type == "UNKNOWN")
            {
                parser.pushValue("NAME", parser.get(1).value);
                parser.advance();
            }
            else
            {

            }
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "RPARENT")
        {
            parser.pushType("RPARENT");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "MINUS" && parser.get(1).type == "GREATER")
        {
            parser.pushType("TO");

            parser.pushValue("TYPE", parser.get(2).value);

            parser.storeToken();
            parser.clearToken();

            parser.advance(2);
        }
        else if (parser.get().type == "LCURLYBR")
        {
            parser.pushType("OPEN");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "RCURLYBR")
        {
            parser.pushType("CLOSE");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "EQUALS")
        {
            if (parser.get(1).type == "EQUALS")
            {
                // Condition
            }
            else
            {
                parser.pushType("EQUAL");
                std::string str = "";
                parser.advance();
                while (parser.get().type != "EOL")
                {
                    if (parser.get().type == "STR")
                        str += "\"" + parser.get().value + "\"";
                    else if (parser.get().type == "POINTER")
                        str += "*";
                    else if (parser.get().type == "REFERENCE")
                        str += "&";
                    else if (parser.get().type == "PERIOD" && parser.get(-1).type == "UNKNOWN" && parser.get(1).type == "UNKNOWN")
                        str += "__";
                    else
                        str += parser.get().value;
                    parser.advance();
                }
                parser.pushValue("RIGHT", str);
                parser.storeToken();
                parser.clearToken();
            }
        }
        else if (parser.get().type == "COMMA")
        {
            parser.pushType("COMMA");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "IF")
        {
            parser.pushType("IF");
            std::string str = "";
            parser.advance();
            while (parser.get().type != "LCURLYBR" && parser.get().type != "THEN")
            {
                if (parser.get().type == "AND")
                    str += " && ";
                else if (parser.get().type == "OR")
                    str += " || ";
                else if (parser.get().type == "STR")
                    str += "\"" + parser.get().value + "\"";
                else if (parser.get().type == "POINTER")
                    str += "*";
                else if (parser.get().type == "REFERENCE")
                    str += "&";
                else
                    str += parser.get().value;
                parser.advance();
            }
            parser.advance(-1);
            parser.pushValue("CONDITION", str);
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "THEN")
        {

        }
        else if (parser.get().type == "RETURN")
        {
            parser.pushType("RETURN");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "PERIOD")
        {
            parser.pushType("PRERIOD");
            parser.storeToken();
            parser.clearToken();
        }
        parser.advance();
    }

    std::string buf = "";
    for (int x = 0; x < parser.comp_tokens.size(); x++)
    {
        if (parser.comp_tokens.at(x).type == "INCLUDE")
        {
            buf += "#include <";
            buf += parser.comp_tokens.at(x).value.at("value");
            buf += ">\n";
        }
        else if (parser.comp_tokens.at(x).type == "Function")
        {
            int y = x+1;
            while (parser.comp_tokens.at(y).type != "RPARENT")
            {
                y++;
            }
            if (parser.comp_tokens.at(y+1).value.at("TYPE") == "?")
            {
                buf += "void ";
            }
            else
            {
                buf += parser.comp_tokens.at(y+1).value.at("TYPE") + " ";
            }
            buf += parser.comp_tokens.at(x).value.at("value");
            buf += "(";
        }
        else if (parser.comp_tokens.at(x).type == "Declaration")
        {
            buf += parser.comp_tokens.at(x).value.at("TYPE");
            buf += parser.comp_tokens.at(x).value.at("value");
            if (parser.comp_tokens.at(x+1).type == "COMMA")
            {
                buf += ", ";
            }
            else if (parser.comp_tokens.at(x+1).type == "Declaration" || parser.comp_tokens.at(x+1).type == "CLOSE")
            {
                buf += ";\n";
            }
            
        }
        else if (parser.comp_tokens.at(x).type == "RPARENT")
        {
            buf += ")";
        }
        else if (parser.comp_tokens.at(x).type == "OPEN")
        {
            buf += "{\n";
        }
        else if (parser.comp_tokens.at(x).type == "CLOSE")
        {
            buf += "};\n";
        }
        else if (parser.comp_tokens.at(x).type == "DATA")
        {
            buf += "struct ";
            buf += parser.comp_tokens.at(x).value.at("NAME");
        }
        else if (parser.comp_tokens.at(x).type == "Call")
        {
            buf += parser.comp_tokens.at(x).value.at("value");
            buf += "(";
            buf += parser.comp_tokens.at(x).value.at("args");
            buf += ");\n";
        }
        else if (parser.comp_tokens.at(x).type == "EQUAL")
        {
            if (parser.comp_tokens.at(x-1).type == "RETURN")
            {
                buf += "return ";
                buf += parser.comp_tokens.at(x).value.at("RIGHT");
                buf += ";\n";
            }
            else
            {
                buf += " = ";
                buf += parser.comp_tokens.at(x).value.at("RIGHT");
                buf += ";\n";
            }
        }
        else if (parser.comp_tokens.at(x).type == "IF")
        {
            buf += "if (";
            buf += parser.comp_tokens.at(x).value.at("CONDITION");
            buf += ")";
        }
    }

    
    std::ofstream out("output.c");
    out << buf;
    out.close();
    return 0;
}
