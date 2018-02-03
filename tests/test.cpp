#include "../tinyconf.hpp"

int main(int argc, char **argv)
{
    stb::Config test("./test.cfg");
    std::vector<int> array = {1, 2, 3, 4};
    std::vector<int> sArray;
    std::string str("OK");
    int integer = 1;
    double decimal = 3.14159265359f;

    test.set("CharTest", "OK");
    test.set("StringTest", str);
    test.set("IntTest", integer);
    test.set("DoubleTest", decimal);
    test.setArray("ArrayTest", array);
    test.save();
    test.relocate("./test.cfg");
    test.getArray<std::vector<int>>("ArrayTest", sArray);
    std::cout << "Array test:";
    for (size_t i = 0; i < sArray.size(); i++)
        std::cout << sArray[i] << ":";
    std::cout << "\n";
    return (0);
}