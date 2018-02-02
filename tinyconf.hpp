#ifndef TINYCONF_HPP_
#define TINYCONF_HPP_

/* * * * * * * * * * * * * * *
 * TinyConf Library
 * Maintained by Stalker2106
 * Version 0.1
 * * * * * * * * * * * * * * */

#include <string>
#include <fstream>
// Stl Containers
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <forward_list>
#include <unordered_set>
#include <unordered_map>

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
     * Used to set configuration values with value of type T.
     */
    template <typename T>
    void set(const std::string &key, const T &value, bool serialize = false)
    {

    }

    /* setContainer(const std::string &key), no arguments
     * sets value of key argument
     * Used to set configuration values with value of type T.
     */
    template <typename T>
    void setContainer(const std::string &key, const T &value, bool serialize = false)
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
            return (false); //No config
        }
        while (std::getline(file, buffer))
        {
            _config.emplace(buffer.substr(0, buffer.find('=')), buffer.substr(buffer.find('=') + 1, buffer.size() - buffer.find('=')));
        }
        file.close();
        return (true);
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
        return (true);
    }

protected:
    std::map<std::string, std::string> _config;
    std::string _path;
};

    /* set specilization for following stl containers:
     * vector
     * list
     * queue
     * deque
     * set
     * multiset
     * map
     * multimap
     * forward_list
     * unordered_set
     * unordered_multiset
     * unordered_map
     */
    template <>
    void Config::set<std::vector>(const std::string &key, const std::vector &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::list>(const std::string &key, const std::list &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::queue>(const std::string &key, const std::deque &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::deque>(const std::string &key, const std::deque &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::set>(const std::string &key, const std::set &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::multiset>(const std::string &key, const std::multiset &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::map>(const std::string &key, const std::map &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::multimap>(const std::string &key, const std::multimap &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::forward_list>(const std::string &key, const std::forward_list &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::unordered_set>(const std::string &key, const std::unordered_set &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::unordered_multiset>(const std::string &key, const std::unordered_multiset &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::unordered_map>(const std::string &key, const std::unordered_map &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::set<std::unordered_multimap>(const std::string &key, const std::unordered_multimap &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }

}

#endif /* !TINYCONF_HPP_ */