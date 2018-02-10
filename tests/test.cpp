#include "../include/tinyconf/tinyconf.hpp"

#include <deque>

void saveAndReload(stb::Config &test)
{   
    std::cout << "Serializing to file \"" << test.getPath() << "\"\n";
    test.save();

    test.relocate("./test.cfg");  
    std::cout << "Relocating to file \"" << test.getPath() << "\"\n";
}

void primitives_tests(stb::Config &test)
{
    char *sCharstr = new char[3], charstr[] = "ISOK";
    std::string sStr, str("OK");
    char sCharc, charc = 'o';

    int16_t sInteger16, integer16 = 2;
    int32_t sInteger32, integer32 = 3;
    int64_t sInteger64, integer64 = 4;
    float sDecimal, decimal = 3.141592f;
    double sDDecimal, dDecimal = 3.1415926535f;

    std::cout << "Setting primitives into memory\n";

    test.set("CharArrayTest", charstr);
    test.set("StringTest", str);
    test.set("CharTest", charc);
    test.set("Int16Test", integer16);
    test.set("Int32Test", integer32);
    test.set("Int64Test", integer64);
    test.set("FloatTest", decimal);
    test.set("DoubleTest", dDecimal);
    
    saveAndReload(test);

    std::cout << "Char Array Test => ";
    test.get("CharArrayTest", sCharstr);
    std::cout << (strcmp(sCharstr, charstr) == 0 ? "OK" : "FAIL") << "\n";

    std::cout << "String Test => ";
    test.get("StringTest", sStr);
    std::cout << (sStr == str ? "OK" : "FAIL") << "\n";

    std::cout << "Char Test => ";
    test.get("CharTest", sCharc);
    std::cout << (sCharc == charc ? "OK" : "FAIL") << "\n";
    
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
}

void containers_tests(stb::Config &test)
{

    std::vector<int> sVectori, vectori = {1, 2, 3};
    std::deque<double> sDeque, deque = { 1.9, 1.23456789, 0.777, 420};
    std::pair<int, float> sPair, pair = {42, 5.12345f};

    std::cout << "Setting containers into memory\n";

    test.setArray("VectorTest", vectori);
    test.setArray("DequeTest", deque);
    test.setPair("PairTest", pair);
 
    saveAndReload(test);

    std::cout << "Vector<int> Test => ";
    test.getArray("VectorTest", sVectori);
    std::cout << (sVectori == vectori ? "OK" : "FAIL") << "\n";

    std::cout << "Deque<double> Test => ";
    test.getArray("DequeTest", sDeque);
    std::cout << (sDeque == deque ? "OK" : "FAIL") << "\n";


    std::cout << "Pair Test => ";
    test.getPair("PairTest", sPair);
    std::cout << (sPair.first == pair.first && sPair.second == pair.second ? "OK" : "FAIL") << "\n";
}

int main(int argc, char **argv)
{
    stb::Config test("./test.cfg");

    std::cout << "#####\nTinyConf Tests Program\n#####\n\n";
    std::cout << "#Primitives tests:\n";
    primitives_tests(test);
    std::cout << "\n#Containers tests:\n";
    containers_tests(test);
    std::cout << "\nTests done!\n";
    return (0);
}