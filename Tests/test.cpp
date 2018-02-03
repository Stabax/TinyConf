#include "../tinyconf.hpp"

int main(int argc, char **argv)
{
    stb::Config test("./test.cfg");

    test.set("CALME", "SOULED");
    test.save();
    return (0);
}