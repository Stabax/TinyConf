# Getting Started #

## Using tinyConf in your projects ##

 1. include tinyconf.hpp
 2. ???
 3. Profit

### The stb::Config class ###

The stb::Config class represent a Configuration.\n
To create a configuration just construct it with the desired path:

    stb::Config cfg("./path/to/file.cfg");

### Basic Get/Set mechanism ###

You can therefore fill it with values by using its set method:

    int val = 5;
    cfg.set("MyVal", val); //Store "MyVal" key in config with value of 5

Grab them back anytime with get:

    int val;
    cfg.get("MyVal", val);
    std::cout << "val was " << val << " !\n"; //Produces "val was 5 !"

Or throw it away with erase:

    cfg.erase("MyVal"); //Removes "MyVal" key from config

### Serialization ###

When you feel like saving your keys to file, just use the save method:

    cfg.save(); //Writes to file

### Multi-Value support ###

Alright, too easy for your C++ master level ?\n
Getting and setting values works for any STL container and std::pair, for example
the following code is valid

    std::vector<int> vec = {0, 1, 2, 3, 4};
    std::vector<int> copy;
 
    cfg.set("MyVector", vec);
    cfg.get("MyVector", copy); //copy equals {0, 1, 2, 3, 4}

### Remove config file ###

Now to erase the config file, the destroy method is available:\n
(please note the config stays the same in memory)

    cfg.destroy();

### Comments in config file ###

TinyConf is non-destructive, and supports comments in configuration files.\n
The parser is non-destructive and keep comments untouched.\n
The syntax allows the following comment-styles:

    Key=Value #Comment
    Key2=Value ;Comment
    /*
     * Multi Line Comment
     */

There is nothing more to learn to start using TinyConf !\n
More advanced methods are nevertheless available and documented in the stb::Class documentation page.