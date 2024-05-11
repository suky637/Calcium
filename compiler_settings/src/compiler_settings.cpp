#include "compiler_settings.h"

CompilerSettings::CompilerSettings(int argc, char** argv)
{
    this->argc = argc;
    for (int i = 0; i < sizeof(argv); i++)
    {
        this->argv.push_back(argv[i]);
    }
}

bool CompilerSettings::setting(std::string arg, bool asArgs, std::string* argRet)
{
    for (int i = 0; i < argv.size(); i++)
    {
        if (argv.at(i) != arg)
            continue;
        if (!asArgs)
            return true;
        if (argv.size()-1 >= i)
        {
            std::cerr << "ERROR, " << arg << " needs an argument.\n";
            return false;
        }
        else
        {
            *argRet = argv.at(i+1);
            return true;
        }
        
    }
    return false;
}

