#include <iostream>
#include <fstream>
#include <lexer.h>
#include <parser.h>
#include <filesystem>

/*
    Suky_Laplante

    CALCIUM PROGRAMMING LANGUAGE

    FILE EXTENSION: *.ca

    VERSION: 1.0-InDev
*/

//#define DEBUG_MODE 1
//#define BUILD_MODE


void compile(std::string file_name)
{
    std::string out_fn = file_name;
    std::string out_bak = file_name;
    int pos_filePath = file_name.find_last_of('.');
    if (pos_filePath != std::string::npos)
    {
        out_bak = file_name.substr(0, pos_filePath) + ".bak";
        out_fn = file_name.substr(0, pos_filePath) + ".c";
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
    lexer.add_token("ELSE", "else");
    lexer.add_token("AND", "and");
    lexer.add_token("OR", "or");
    lexer.add_token("DATA", "data");
    lexer.add_token("NAMESPACE", "namespace");
    lexer.add_token("ENDNAMESPACE", "endnamespace");
    lexer.add_token("STRUCT", "struct");
    //lexer.add_token("FOR", "for"); // TODO
    lexer.add_token("WHILE", "while");
    lexer.add_token("SAFEFN", "safefn");
    lexer.add_token("DELETE", "delete");
    lexer.add_token("DELETE_END", "delete_end");
    lexer.add_token("KEEP", "keep");
    lexer.add_token("CONTINUE", "continue");
    lexer.add_token("BREAK", "break");
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // PARSING
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    std::string currentNamespace = "";
    while (!parser.end())
    {
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // IMPORTS
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        if (parser.get().type == "IMPORT")
        {
            parser.pushType("INCLUDE");
            
            #ifdef BUILD_MODE
            #define LIB_PATH "../lib/"
            #else
            #define LIB_PATH "/usr/lib/calcium/lib/"
            #endif
            if (std::filesystem::exists(LIB_PATH + parser.get(1).value))
            {

                // value: not-compiled
                // real-value: compiled

                int lastInd = parser.get(1).value.find_last_of('.');
                parser.pushValue("realValue", std::filesystem::absolute(LIB_PATH + parser.get(1).value).generic_string());
                std::string extension = parser.get(1).value.substr(lastInd + 1);
                    if (extension == "ca")
                    {
                        parser.pushValue("value", std::filesystem::absolute(LIB_PATH + parser.get(1).value.substr(0, lastInd) + ".c"));
                        parser.pushValue("type", "ca");
                    }
                    else
                    {
                        parser.pushValue("type", "unknown");
                    }
            }
            else
            {
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
                    else if (extension == "ca")
                    {
                        parser.pushValue("value", parser.get(1).value.substr(0, lastInd) + ".c");
                        parser.pushValue("type", "ca");
                    }
                    else
                    {
                        parser.pushValue("type", "unknown");
                    }
                }
            }
            parser.advance();
            parser.storeToken();
            parser.clearToken();
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // NAMESPLACE
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (parser.get().type == "NAMESPACE")
        {
            currentNamespace = parser.get(1).value + "__";
            parser.advance();
        }
        else if (parser.get().type == "ENDNAMESPACE")
        {
            currentNamespace = "";
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // VARIABLES | FUNCTIONS
        /////////////////////////////////////////////////////////////////////////////////////////////////////
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
            else if (parser.get(1).type == "LBRAQ")
            {
                parser.pushType("Declaration");
                parser.pushValue("value", parser.get().value + "[" + parser.get(2).value + "]");
                parser.advance(2);
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
                    {
                        parser.pushValue("pointer", "1");
                        args += "*";
                    }
                    else if (parser.get(ind).type == "DIGIT")
                    {
                        parser.pushValue("array", parser.get(ind).value);
                    }
                    else if (parser.get(ind).type == "NULL")
                    {
                        parser.pushValue("array", "NULL");
                    }
                    else if (parser.get(ind).type == "KEEP")
                    {
                        parser.pushValue("not", "1");
                    }
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
                else if (parser.get(-1).type == "SAFEFN")
                {
                    parser.pushType("Safe Function");
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
                    int current = 0;
                    while (parser.get().type != "RPARENT" || current != 0)
                    {
                        if (parser.get().type == "STR")
                            args += "\"" + parser.get().value + "\"";
                        else if (parser.get().type == "POINTER")
                            args += "*";
                        else if (parser.get().type == "REFERENCE")
                            args += "&";
                        else if (parser.get().type == "DIGIT")
                            args += parser.get().value;
                        else if (parser.get().type == "UNKNOWN")
                            args += parser.get().value;
                        else if (parser.get().type == "LBRAQ")
                            args += "[";
                        else if (parser.get().type == "RBRAQ")
                            args += "]";
                        else if (parser.get().type == "LPARENT")
                        {
                            args += "(";
                            current++;
                        }
                        else if (parser.get().type == "RPARENT")
                        {
                            args += ")";
                            current--;
                        }
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
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // STRUCTS
        /////////////////////////////////////////////////////////////////////////////////////////////////////
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
        // no one cares about that
        else if (parser.get().type == "RPARENT")
        {
            parser.pushType("RPARENT");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "CONTINUE")
        {
            parser.pushType("CONTINUE");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "BREAK")
        {
            parser.pushType("BREAK");
            parser.storeToken();
            parser.clearToken();
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // TO (->) KEYWORD
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (parser.get().type == "MINUS" && parser.get(1).type == "GREATER")
        {
            parser.pushType("TO");

            int x = 1;
            std::vector<std::string> types{};
            while (parser.get(x).type != "LCURLYBR")
            {
                if (parser.get(x).value == "?")
                    types.push_back("void");
                else if (parser.get(x).value == "point")
                    types.push_back("*");
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
        // yet an another thing that no one cares
        else if (parser.get().type == "LCURLYBR")
        {
            parser.pushType("OPEN");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "DELETE")
        {
            parser.pushType("DELETE");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "DELETE_END")
        {
            parser.pushType("DELETE_END");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "RCURLYBR")
        {
            parser.pushType("CLOSE");
            parser.storeToken();
            parser.clearToken();
        }
        else if (parser.get().type == "ELSE")
        {
            parser.pushType("ELSE");
            parser.storeToken();
            parser.clearToken();
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // EQUALITY
        /////////////////////////////////////////////////////////////////////////////////////////////////////
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
        // again? an another symbol that no one cares?
        else if (parser.get().type == "COMMA")
        {
            parser.pushType("COMMA");
            parser.storeToken();
            parser.clearToken();
        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // CONDITION
        /////////////////////////////////////////////////////////////////////////////////////////////////////
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
                else if (parser.get().type == "PERIOD" && parser.get(1).type == "UNKNOWN" && parser.get(2).type == "LPARENT")
                    str += "__";
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
                else if (parser.get().type == "PERIOD" && parser.get(1).type == "UNKNOWN" && parser.get(2).type == "LPARENT")
                    str += "__";
                else
                    str += parser.get().value;
                parser.advance();
            }
            parser.advance(-1);
            parser.pushValue("CONDITION", str);
            parser.storeToken();
            parser.clearToken();
        }
        // useless symbol. litterally the most useless thing in this entire programming language. This token doesn't do anything, yet I still implemented it because I thought it would be cool. the "then" keyword is just a cosmetic keyword that is completely optionnal, and I'm too lazy to remove that useless keyword, but I'm not too lazy to write a text for that.
        else if (parser.get().type == "THEN")
        {

        }
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        // RETURN
        /////////////////////////////////////////////////////////////////////////////////////////////////////
        else if (parser.get().type == "RETURN")
        {
            parser.pushType("RETURN");
            parser.storeToken();
            parser.clearToken();
        }
        // forget about it
        else if (parser.get().type == "PERIOD")
        {
            parser.pushType("PERIOD");
            parser.storeToken();
            parser.clearToken();
        }
        parser.advance();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    // COMPILATION
    /////////////////////////////////////////////////////////////////////////////////////////////////////

    //parser.display_compressed();

    bool isInSafeFN = false;
    std::vector<std::string> variableGotten{};
    std::string buf = "";
    for (int x = 0; x < parser.comp_tokens.size(); x++)
    {
        // Safe Function
        //std::cout << x << std::endl;
        if (parser.comp_tokens.at(x).type == "INCLUDE")
        {
            if (parser.comp_tokens.at(x).value.at("type") == "ca")
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
        else if (parser.comp_tokens.at(x).type == "ELSE")
        {
            buf += "else ";
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
        else if (parser.comp_tokens.at(x).type == "DELETE_END")
        {
            for (auto s : variableGotten)
            {
                buf += "free(";
                buf += s;
                buf += ");\n";
            }
            isInSafeFN = 0;
            variableGotten.clear();
        }
        else if (parser.comp_tokens.at(x).type == "CONTINUE")
        {
            buf += "continue;\n";
        }
        else if (parser.comp_tokens.at(x).type == "BREAK")
        {
            buf += "break;\n";
        }
        else if (parser.comp_tokens.at(x).type == "DELETE")
        {
            for (auto s : variableGotten)
            {
                buf += "free(";
                buf += s;
                buf += ");\n";
            }
        }
        else if (parser.comp_tokens.at(x).type == "Safe Function")
        {
            isInSafeFN = 1;
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
            if (parser.comp_tokens.at(x).value.count("array"))
            {
                buf += "[";
                if (parser.comp_tokens.at(x).value.at("array") != "NULL")
                    buf += parser.comp_tokens.at(x).value.at("array");
                buf += "]";
            }
            if (parser.comp_tokens.at(x).value.count("pointer") && !parser.comp_tokens.at(x).value.count("not") && isInSafeFN)
            {
                variableGotten.push_back(parser.comp_tokens.at(x).value.at("value"));
            }
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
            buf += "}";
            if (x+1 < parser.comp_tokens.size())
                if (parser.comp_tokens.at(x+1).type != "ELSE")
                    buf += ";\n";
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
        return 1;
    }
    else
        file_name = argv[1];
    
    compile(file_name);
    return 0;
}
