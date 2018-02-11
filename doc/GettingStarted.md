# Getting Started #

## Using tinyConf in your projects ##

 1. include tinyconf.hpp
 2. ???
 3. Profit

## The stb::Config class ##

The stb::Config class represent a Configuration.\n
To create a configuration just construct it with the desired path:

    stb::Config cfg("./path/to/file.cfg");

 You can therefore fill it with values by using its set method:

    int val = 5;
    cfg.set("MyVal", val); //Store "MyVal" key in config with value of 5

And get them back anytime with get:

    int val;
    cfg.get("MyVal", val);
    std::cout << "val was " << val << " !\n"; //Produces "val was 5 !"

When you feel like saving your keys to file, just use the save method:

    cfg.save(); //Writes to file

Alright, alright, too easy ?\n
Getting and setting values can work for any STL container or std::pair, for example
the following code is valid

    std::vector<int> vec = {0, 1, 2, 3, 4};
    std::vector<int> copy;
 
    cfg.set("MyVector", vec);
    cfg.get("MyVector", copy); //copy equals {0, 1, 2, 3, 4}

