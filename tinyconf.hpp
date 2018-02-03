#ifndef TINYCONF_HPP_
#define TINYCONF_HPP_

/* * * * * * * * * * * * * * * * * * * * *
 * TinyConf Library
 * @version 0.1
 * @file tinyconf.hpp
 * @brief Single header for Config class
 * @author Maxime 'Stalker2106' Martens
 * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include <fstream>
#include <sstream>
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

#define KEY_VALUE_SEPARATOR ":="
#define VALUE_FIELD_SEPARATOR ":=:"

/* Everything is defined within stb:: scope */
namespace stb {

/*
 * @class Config
 * @brief Main Config class: Defines the whole library more or less
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

    /*
     * @fn T get(const std::string &key)
     * @brief Used to get values from configuration
     * @param key : The key identifying wanted value
     * @return value associated with key, of T type
     */
    template <typename T>
    T get(const std::string &key)
    {
        T value;
        if (_config.find(key) != _config.end())
        {
            std::istringstream iss;
            iss.str(_config[key]);
            iss >> value;
            return (value);
        }
        else
        {
            return (value);
        }
    }

    /* 
     * @fn void set(const std::string &key, const std::string &value, bool serialize = false)
     * @brief Used to set configuration values with String/String type.
     * @param key : The key indentifier to set
     * @param value : The formatted string value to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    void set(const std::string &key, const std::string &value, bool serialize = false)
    {
        if (_config.find(key) != _config.end())
        {
            _config[key] = value;
        }
        else
        {
            _config.emplace(key, value);
        }
    }

    /* 
     * @fn void set(const std::string &key, const char *value, bool serialize = false)
     * @brief Used to set configuration values with a char array.
     * @param key : The key indentifier to set
     * @param value : The char array to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    void set(const std::string &key, const char *value, bool serialize = false)
    {
        set(key, std::string(value), serialize);
    }

    /* 
     * @fn void set(const std::string &key, const T &value, bool serialize = false)
     * @brief Used to set configuration values with any primitive type.
     * @param key : The key indentifier to set
     * @param value : The primitive-typed value to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template <typename T>
    void set(const std::string &key, const T &value, bool serialize = false)
    {
        std::string fValue = std::to_string(value);

        set(key, fValue, serialize);
    }

    /* 
     * @fn void setContainer(const std::string &key, const T &value, bool serialize = false)
     * @brief Used to set configuration values with an stl container.
     * @param key : The key indentifier to set
     * @param value : The container with values to fill in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template <typename T>
    void setContainer(const std::string &key, const T &value, bool serialize = false)
    {
        std::string fValue;

        for (typename T::const_iterator it = value.begin(); it != value.end(); it++)
        {
            if (it != value.begin())
            {
                fValue += VALUE_FIELD_SEPARATOR;
            }
            fValue += std::to_string(*it);
        }
        set(key, fValue,serialize);
    }

    /* 
     * @fn bool load()
     * @brief Used to load associated the config file.
     * @return true on success, false on failure.
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
            size_t separator = buffer.find(KEY_VALUE_SEPARATOR);
            if (separator == std::string::npos) continue;
            _config.emplace(buffer.substr(0, separator), buffer.substr(separator + 1, buffer.size() - separator));
        }
        file.close();
        return (true);
    }


    /* 
     * @fn bool save() const
     * @brief Used to save current state inside the config file.
     * @return true on success, false on failure.
     */
    bool save() const
    {
        std::ofstream file(_path, std::ofstream::out | std::ofstream::trunc);

        if (!file.good())
        {
            return (false); //Couldnt open
        }
        for (std::map<std::string, std::string>::const_iterator it = _config.begin(); it != _config.end(); it++)
        {
            file << (it)->first + KEY_VALUE_SEPARATOR + (it)->second << "\n";
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
    template <>
    void Config::Config::set<std::vector>(const std::string &key, const std::vector &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::list>(const std::string &key, const std::list &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::queue>(const std::string &key, const std::deque &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::deque>(const std::string &key, const std::deque &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::set>(const std::string &key, const std::set &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::multiset>(const std::string &key, const std::multiset &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::map>(const std::string &key, const std::map &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::multimap>(const std::string &key, const std::multimap &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::forward_list>(const std::string &key, const std::forward_list &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::unordered_set>(const std::string &key, const std::unordered_set &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::unordered_multiset>(const std::string &key, const std::unordered_multiset &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::unordered_map>(const std::string &key, const std::unordered_map &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    template <>
    void Config::Config::set<std::unordered_multimap>(const std::string &key, const std::unordered_multimap &value, bool serialize = false)
    {
        setContainer(key, value, serialize);
    }
    */

}

#endif /* !TINYCONF_HPP_ */