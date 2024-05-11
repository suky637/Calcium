#include <string>
#include <vector>
#include <iostream>

class CompilerSettings
{
    public:
    CompilerSettings(int argc, char** argv);
    bool setting(std::string arg, bool asArgs, std::string* argRet);
    private:
    int argc;
    std::vector<std::string> argv{};
};