#include "../tinyconf.hpp"

int main(int argc, char **argv)
{
    stb::Config test("./test.cfg");
    std::vector<int> sArray, array = {1, 2, 3};
    std::pair<int, float> sPair, pair = {42, 5.12345f};
    char *sChar = new char[3];
    std::string sStr, str("OK");
    int sInteger, integer = 1;
    double sDecimal, decimal = 3.1415926535f;

    std::cout << "TinyConf Tests:\n";
    std::cout << "Setting tests into config\n";

    test.set("CharTest", "OK");
    test.set("StringTest", str);
    test.set("IntTest", integer);
    test.set("DoubleTest", decimal);
    test.setArray("ArrayTest", array);
    test.setAggregate("PairTest", pair);

    std::cout << "Serializing to file \"" << test.getPath() << "\"\n";
    test.save();

    test.relocate("./test.cfg");  
    std::cout << "Reloading file \"" << test.getPath() << "\"\n";

    std::cout << "Char Test => ";
    test.get("CharTest", sChar);
    std::cout << (strcmp(sChar, "OK") == 0 ? "OK" : "FAIL") << "\n";

    std::cout << "String Test => ";
    test.get("StringTest", sStr);
    std::cout << (sStr == "OK" ? "OK" : "FAIL") << "\n";

    std::cout << "Int Test => ";
    test.get("IntTest", sInteger);
    std::cout << (sInteger == 1 ? "OK" : "FAIL") << "\n";

    std::cout << "Double Test => ";
    test.get("DoubleTest", sDecimal);
    std::cout << (sDecimal == 3.141592f ? "OK" : "FAIL") << "\n";

    std::cout << "Vector Test => ";
    test.getArray("ArrayTest", sArray);
    std::cout << (sArray[0] == 1 && sArray[1] == 2 && sArray[2] == 3 ? "OK" : "FAIL") << "\n";

    std::cout << "Pair Test => ";
    test.getAggregate("PairTest", sPair);
    std::cout << (sPair.first == 42 && sPair.second == 5.12345f ? "OK" : "FAIL") << "\n";
    
    return (0);
}