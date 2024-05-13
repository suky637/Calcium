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
    //lexer.add_delimiter("QUOTE", '"');
    lexer.add_delimiter("SMOL_QUOTE", '\'');
    lexer.add_delimiter("LBRAQ", '[');
    lexer.add_delimiter("RBRAQ", ']');

    // reserved words
    lexer.add_token("CONST", "const");
    lexer.add_token("LOCK", "lock");
    lexer.add_token("INT", "int");
    lexer.add_token("STRING", "string");
    lexer.add_token("CHAR", "char");
    lexer.add_token("ANY", "any");
    lexer.add_token("RETURN", "return");
    lexer.add_token("BOOLEAN", "bool");
    lexer.add_token("FLOAT", "float");
    lexer.add_token("DOUBLE", "double");
    lexer.add_token("STRING", "string");
    lexer.add_token("LOCAL", "local");
    lexer.add_token("MATH_FUNC", "func");
    lexer.add_token("FUNCTION", "fn");
    lexer.add_token("RETURN", "return");
    lexer.add_token("BOOLEAN", "bool");
    lexer.add_token("FLOAT", "float");
    lexer.add_token("DOUBLE", "double");
    lexer.add_token("IMPORT", "import");

    


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

    lexer.print_tokens();

    Parser parser{};

    parser.add_to_blacklist("CONST");
    parser.add_to_blacklist("LOCK");
    parser.add_to_blacklist("FUNC");
    parser.add_to_blacklist("FN");
    parser.add_to_blacklist("LOCAL");
    parser.add_to_blacklist("SPACE");
    parser.add_to_blacklist("TAB");
    parser.add_to_blacklist("COMMA");
    //parser.add_to_blacklist("QUOTE");


    parser.compress(lexer.tokens);
    //parser.display_compressed();
    std::string res = parser.compile();

     // Specify the file name
               {

            } std::string filename = "output.c";
    std::ofstream outputFile(filename);

    // Check if the file is opened successfully
    if (outputFile.is_open()) {
        outputFile << res;
        outputFile.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }


    return 0;
}
