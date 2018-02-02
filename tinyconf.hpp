#ifndef TINYCONF_HPP_
#define TINYCONF_HPP_

/* * * * * * * * * * * * * * *
 * TinyConf Library
 * Maintained by Stalker2106
 * Version 0.1
 * * * * * * * * * * * * * * */

#include <string>
#include <fstream>
#include <map>

/* Everything is defined within stb:: scope */
namespace stb {

/* Main Config class
 * Defines the whole library more or less;
 */
class Config
{
public:
    Config(const std::string &path) : _path(path)
    {

    }

    ~Config()
    {

    }

    /* get(const std::string &key), string key argument
     * returns value of key argument with T type
     * Used to access configuration values.
     */
    template <typename T>
    T get(const std::string &key) const
    {

    }

    /* set(const std::string &key), no arguments
     * sets value of key argument
     * Used to set configuration values.
     */
    template <typename T>
    void set(const std::string &key, T value)
    {

    }

    /* load(), no arguments
     * returns true on success, false on failure.
     * Used to load associated the config file.
     */
    bool load()
    {
        std::ifstream file(_path, std::ifstream::in);
        std::string buffer;

        if (!file.good())
        {
            return; //No config
        }
        while (std::getline(file, buffer))
        {
            _config.emplace(buffer.substr(0, buffer.find('=')), buffer.substr(buffer.find('=') + 1, buffer.size() - buffer.find('=')));
        }
        file.close();
    }

    /* save(), no arguments
     * returns true on success, false on failure.
     * Used to save current state inside the config file.
     */
    bool save() const
    {
        std::ofstream file(_path, std::ofstream::out | std::ofstream::trunc);

        if (!file.good())
        {
            return (false); //Couldnt open
        }
        for (std::map<std::string, std::string>::iterator it = _config.begin(); it != _config.end(); it++)
        {
            file << (it)->first + "=" + (it)->second << "\n";
        }
        file.close();
    }

protected:
    std::string _path;
};

}

#endif /* !TINYCONF_HPP_ */