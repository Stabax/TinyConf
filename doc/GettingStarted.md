# Getting Started #

## Using tinyConf in your projects ##

 1. include tinyconf.hpp
 2. ???
 3. Profit

### The stb::Config class ###

The stb::Config class represent a Configuration.\n
To create a configuration just construct it with the desired path:

    stb::Config cfg("./path/to/file.cfg");

Or if you want you can create it and bind it to a file later:\n
(Please note you can't save, load or reload config in this state until you set path)

    stb::Config cfg;

    cfg.setPath("./file.cfg"); //Associates a file for serialization

### Basic Get/Set mechanism ###

You can fill configuration with values by using its set method:

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

You can reload the file from disk into config:\n
(warning: this will erase any unsaved change)

    cfg.reload(); //Update config values from file

### Multi-Value support ###

Alright, too easy for your C++ master level ?\n
Getting and setting values works for any STL container and std::pair, for example
the following code is valid

    std::vector<int> vec = {0, 1, 2, 3, 4};
    std::vector<int> copy;
 
    cfg.setContainer("MyVector", vec);
    cfg.getContainer("MyVector", copy); //copy equals {0, 1, 2, 3, 4}

### Sections of keys ###

You can use Sections to order keys, and store duplicate keys in config.
Each section acts as a namespace, and is reachable via get methods with a simple operator

    cfg.set("Section:Key", "Value"); //Sets the Key in Section with Value

    std::string value;
    cfg.get("Section:Key", value); //Fills value with Key of Section value

### Reserved characters ###

The library forbids the use of certain characters in keys, values or sections.\n
By default, the following chars are not allowed : ", ', [, ], #, ;, /*, */, :, =\n
You can still use the characters by escaping them with the defined escape char.\n
for example:

    this \#key isn\'t invalid=this aint \; \*/ either   //Note that the */ sequence was escaped with a single escape char

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

### Customize syntax ###

You can customize every single "control" character or sequence that is used to recognize Keys/Values, Multivalues, Comments, Sections...\n
To do so, you can check the defines inside tinyconf.config.hpp file which are documented as they are defined.\n

### Example configuration file ###

    /*
     * TinyConf example configuration file
     */
    StringTest=OK
    DequeTest=1.900000:1.234568:0.777000:420.000000
    PairTest=42:5.123450
    [Serious Section]
    VectorTest=1:2:3
    LineComment1=WORKSFORME# Basic Comment



There is nothing more to learn to start using TinyConf !\n
More advanced methods are nevertheless available and documented in the stb::Class documentation page.