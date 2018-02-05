#include "../include/tinyconf/tinyconf.hpp"

int main(int argc, char **argv)
{
    stb::Config test("./test.cfg");
    std::vector<int> sArray, array = {1, 2, 3};
    std::pair<int, float> sPair, pair = {42, 5.12345f};
    char *sChar = new char[3];
    std::string sStr, str("OK");
    int8_t sInteger8, integer8 = 1;
    int16_t sInteger16, integer16 = 2;
    int32_t sInteger32, integer32 = 3;
    int64_t sInteger64, integer64 = 4;
    float sDecimal, decimal = 3.141592f;
    double sDDecimal, dDecimal = 3.1415926535f;

    std::cout << "TinyConf Tests:\n";
    std::cout << "Setting tests into config\n";

    test.set("CharTest", "OK");
    test.set("StringTest", str);
    test.set("Int8Test", integer8);
    test.set("Int16Test", integer16);
    test.set("Int32Test", integer32);
    test.set("Int64Test", integer64);
    test.set("FloatTest", decimal);
    test.set("DoubleTest", dDecimal);
    test.setArray("ArrayTest", array);
    test.setPair("PairTest", pair);

    std::cout << "Serializing to file \"" << test.getPath() << "\"\n";
    test.save();

    test.relocate("./test.cfg");  
    std::cout << "Reloading file \"" << test.getPath() << "\"\n";

    std::cout << "Char Test => ";
    test.get("CharTest", sChar);
    std::cout << (strcmp(sChar, "OK") == 0 ? "OK" : "FAIL") << "\n";

    std::cout << "String Test => ";
    test.get("StringTest", sStr);
    std::cout << (sStr == str ? "OK" : "FAIL") << "\n";

    std::cout << "Int 8 Test => ";
    test.get("Int8Test", sInteger8);
    std::cout << (sInteger8 == integer8 ? "OK" : "FAIL") << "\n";
    
    std::cout << "Int 16 Test => ";
    test.get("Int16Test", sInteger16);
    std::cout << (sInteger16 == integer16 ? "OK" : "FAIL") << "\n";
    
    std::cout << "Int 32 Test => ";
    test.get("Int32Test", sInteger32);
    std::cout << (sInteger32 == integer32 ? "OK" : "FAIL") << "\n";
    
    std::cout << "Int 64 Test => ";
    test.get("Int64Test", sInteger64);
    std::cout << (sInteger64 == integer64 ? "OK" : "FAIL") << "\n";

    std::cout << "Float Test => ";
    test.get("FloatTest", sDecimal);
    std::cout << (sDecimal == decimal ? "OK" : "FAIL") << "\n";

    std::cout << "Double Test => ";
    test.get("DoubleTest", sDDecimal);
    std::cout << (sDDecimal == dDecimal ? "OK" : "FAIL") << "\n";

    std::cout << "Vector Test => ";
    test.getArray("ArrayTest", sArray);
    for (size_t i = 0; i < sArray.size(); i++)
    {
        if (sArray[i] != array[i])
        {
            std::cout << "FAIL\n";
            break;
        }
        if (i == array.size() - 1) std::cout << "OK\n";
    }

    std::cout << "Pair Test => ";
    test.getPair("PairTest", sPair);
    std::cout << (sPair.first == pair.first && sPair.second == pair.second ? "OK" : "FAIL") << "\n";
    
    return (0);
}