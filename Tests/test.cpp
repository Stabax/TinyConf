#include "../tinyconf.hpp"

int main(int argc, char **argv)
{
    stb::Config test("./test.cfg");
    std::vector<int> array = {1, 2, 3, 4};
    std::string str("OK");
    int integer = 1;
    double decimal = 3.14159265359f;

    test.set("CharTest", "OK");
    test.set("StringTest", str);
    test.set("IntTest", integer);
    test.set("DoubleTest", decimal);
    //test.set("ArrayTest", array);
    test.save();
    return (0);
}