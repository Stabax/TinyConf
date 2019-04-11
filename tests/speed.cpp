#include "../include/tinyconf/tinyconf.hpp"

#include <iostream>

void get_loop(stb::Config &cfg, size_t it)
{
    while (it-- > 0)
    {
        //cfg.set("Test", "0123456789");
        //cfg.get<std::string>("Test");
    }
}

int main(int argc, char **argv)
{
    stb::Config cfg;

    std::cout << "#####\nTinyConf Speed Test\n#####\n\n";
    std::cout << "#100.000 set/get runs... ";
    get_loop(cfg, 1000000);
    std::cout << "DONE\n";
    return (0);
}