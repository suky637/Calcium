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

#define DEBUG_MODE 1


void compile(std::string file_name)
{
    
    std::string out_fn = file_name;
    int pos_filePath = file_name.find_last_of('.');
    if (pos_filePath != std::string::npos)
        out_fn = file_name.substr(0, pos_filePath) + ".c";

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
    lexer.add_token("INT", "int");
    lexer.add_token("CHAR", "char");
    lexer.add_token("RETURN", "return");
    lexer.add_token("BOOLEAN", "bool");
    lexer.add_token("FLOAT", "float");
    lexer.add_token("DOUBLE", "double");
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
    lexer.add_token("STRUCT", "struct");
    //lexer.add_token("FOR", "for"); // TODO
    lexer.add_token("WHILE", "while"); // TODO
    /////////////////////////////////////////////////////////////////////
    // PROCESSING THE LANGUAGE
    /////////////////////////////////////////////////////////////////////
    
    
    std::ifstream file(file_name);
    std::string line;

    // Check if the file is open
    if (!file.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        exit(1);
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
            // Saying which type of lib it is
            int lastInd = parser.get(1).value.find_last_of('.');
            parser.pushValue("realValue", parser.get(1).value);
            if (lastInd == std::string::npos)
            {
                parser.pushValue("type", "unknown");
            }
            else
            {
                std::string extension = parser.get(1).value.substr(lastInd + 1);
                if (extension == "h")
                {
                    parser.pushValue("value", parser.get(1).value);
                    parser.pushValue("type", "c");
                }
                else if (extension == "c")
                {
                    parser.pushValue("value", parser.get(1).value);
                    parser.pushValue("type", "c");
                }
                else if (extension == "cal")
                {
                    parser.pushValue("value", parser.get(1).value.substr(0, lastInd) + ".c");
                    parser.pushValue("type", "cal");
                }
                else
                {
                    parser.pushValue("type", "unknown");
                }
            }
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
            if (parser.get(1).type == "PERIOD")
            {
                parser.pushType("Declaration");
                std::string args = "";
                while (true)
                {
                    if (parser.get().type != "UNKNOWN" && parser.get().type != "PERIOD")
                        break;
                    else
                        args += parser.get().value;
                    parser.advance();
                }
                parser.advance(-1);
                parser.pushValue("value", args);
                parser.storeToken();
                parser.clearToken();
            }
            else if (parser.get(1).type == "EQUALS" || parser.get(1).type == "NULL")
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
                        else if (parser.get().type == "DIGIT")
                            args += parser.get().value;
                        else if (parser.get().type == "UNKNOWN")
                            args += parser.get().value;
                        else if (parser.get().type == "COMMA")
                            args += ",";
                        else if (parser.get().type == "PERIOD")
                            args += ".";
                        parser.advance();
                    }
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

            int x = 1;
            std::vector<std::string> types{};
            while (parser.get(x).type != "LCURLYBR")
            {
                if (parser.get(x).value == "?")
                    types.push_back("void");
                else
                    types.push_back(parser.get(x).value);
                x++;
            }
            std::string type = "";
            int _x = types.size() - 1;
            if (_x == 0)
                type = types.at(0);
            else
            {
                for (int j = types.size() - 1; j > 0; j--)
                {
                    type += types.at(j) + " ";
                }
            }
            parser.pushValue("TYPE", type);

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
                    else if (parser.get().type == "PERIOD" && parser.get(-1).type == "UNKNOWN" && parser.get(1).type == "UNKNOWN" && parser.get(2).type == "LPARENT")
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

        else if (parser.get().type == "WHILE")
        {
            parser.pushType("WHILE");
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
            parser.pushType("PERIOD");
            parser.storeToken();
            parser.clearToken();
        }
        parser.advance();
    }

    //parser.display_compressed();

    std::string buf = "";
    for (int x = 0; x < parser.comp_tokens.size(); x++)
    {
        //std::cout << x << std::endl;
        if (parser.comp_tokens.at(x).type == "INCLUDE")
        {
            if (parser.comp_tokens.at(x).value.at("type") == "cal")
            {
                buf += "#include \"";
                buf += parser.comp_tokens.at(x).value.at("value");
                buf += "\"\n";
                compile(parser.comp_tokens.at(x).value.at("realValue"));
            }
            else
            {
                buf += "#include <";
                buf += parser.comp_tokens.at(x).value.at("value");
                buf += ">\n";
            }
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
            if (parser.comp_tokens.at(x).value.count("arg"))
                buf += parser.comp_tokens.at(x).value.at("arg") + " this";
            if (parser.comp_tokens.at(x+1).type == "Declaration" && parser.comp_tokens.at(x).value.count("arg"))
                buf += ", ";
        }
        else if (parser.comp_tokens.at(x).type == "Declaration")
        {
            if (parser.comp_tokens.at(x).value.count("TYPE"))
                buf += parser.comp_tokens.at(x).value.at("TYPE");
            buf += parser.comp_tokens.at(x).value.at("value");
            if (parser.comp_tokens.size() <= x+1)
            {
                buf += ";\n";
            }
            else if (parser.comp_tokens.at(x+1).type == "COMMA")
            {
                buf += ", ";
            }
            else if (parser.comp_tokens.at(x+1).type == "RPARENT")
                buf += "";
            else if (parser.comp_tokens.at(x+1).type != "EQUAL")
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
            //std::cout << "Work?\n";
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
        else if (parser.comp_tokens.at(x).type == "WHILE")
        {
            buf += "while (";
            buf += parser.comp_tokens.at(x).value.at("CONDITION");
            buf += ")";
        }
    }
    
    std::ofstream out(out_fn);
    out << buf;
    out.close();
}

int main(int argc, char* argv[])
{
    
    // Make code fast
    //std::ios::sync_with_stdio(false);
    std::string file_name;
    if (argc != 2){
        std::cout << "Usage: " << argv[0] << " <filename>" << "\n";
        std::cin >> file_name;
    }
    else
        file_name = argv[1];
    
    compile(file_name);
    return 0;
}
