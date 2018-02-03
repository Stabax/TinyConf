#ifndef TINYCONF_HPP_
#define TINYCONF_HPP_

/*! * * * * * * * * * * * * * * * * * * * *
 * TinyConf Library
 * @version 0.1
 * @file tinyconf.hpp
 * @brief Single header for Config class
 * @author Maxime 'Stalker2106' Martens
 * * * * * * * * * * * * * * * * * * * * */

#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <type_traits>
#include <iomanip>
// Tmp
#include <iostream>
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
#define DECIMAL_PRECISION 10

/* Everything is defined within stb:: scope */
namespace stb {

/*!
 * @struct has_const_iterator
 * @brief Helper to determine whether there's a const_iterator for T.
 */
template<typename T>
struct has_const_iterator
{
private:
    template<typename C> static char evaluate(typename C::const_iterator*);
    template<typename C> static int  evaluate(...);
public:
    enum { value = sizeof(evaluate<T>(0)) == sizeof(char) };
};

/*
 * @class Config
 * @brief Main Config class: Defines the whole library more or less
 */
class Config
{
public:
    /*!
     * @brief Config default constructor
     * @param path : The path where the file.cfg will reside
     */
    Config(const std::string &path) : _path(path)
    {
        load();
    }

    /*!
     * @brief Used to switch current object to another configuration file
     * @param path : The path to relocate to
     */
    void relocate(const std::string &path)
    {
        _path = path;
        _config.clear();
        load();
    }

    ~Config()
    {

    }

   /*!
     * @brief Used to get path of associated configuration
     * @return String containing the path of the current associated cfg file
     */
     std::string getPath()
     {
         return (_path);
     }

    /*!
     * @brief Used to get values from configuration
     * @param key : The key identifying wanted value
     * @param value : The variable to set with value
     * @return value associated with key, of T type
     */
    template <typename T>
    bool get(const std::string &key, T &value)
    {
        if (_config.find(key) != _config.end())
        {
            std::istringstream iss;
            iss.str(_config[key]);
            iss >> value;
        }
        else
        {
            return (false);
        }
        return (true);
    }

    /*!
     * @brief Used to get values from configuration
     * @param key : The key identifying wanted value
     * @param value : The char array to set with value
     * @return value associated with key, of T type
     */
    bool get(const std::string &key, char *value)
    {
        if (_config.find(key) != _config.end())
        {
            strcpy(value, _config[key].c_str());
        }
        else
        {
            return (false);
        }
        return (true);
    }

    /*!
     * @brief Used to get values from configuration
     * @param key : The key identifying wanted value
     * @param value : The string to set with value
     * @return value associated with key, of T type
     */
    bool get(const std::string &key, std::string &value)
    {
        if (_config.find(key) != _config.end())
        {
            value = _config[key];
        }
        else
        {
            return (false);
        }
        return (true);
    }

    /*!
     * @brief Used to get values from configuration array
     * @param key : The key identifying wanted array of values
     * @param container : The container where the array of values will be pushed
	 * @return true on success, false on failure.
     */
    template <typename T>
    bool getArray(const std::string &key, T &container)
    {
        if (_config.find(key) != _config.end())
        {
            std::istringstream iss;
            typename T::value_type value;
            std::string buffer = _config[key];

            for (size_t sep = buffer.find(VALUE_FIELD_SEPARATOR); sep != std::string::npos; sep = buffer.find(VALUE_FIELD_SEPARATOR))
            {
                iss.str(buffer.substr(0, sep));
                iss >> value;
                container.insert(container.end(), value);
                buffer.erase(0, sep + strlen(VALUE_FIELD_SEPARATOR));
                iss.clear();
            }
            iss.str(buffer);
            iss >> value;
            container.insert(container.end(), value);
        }
        else
        {
            return (false);
        }
		return (true);
    }

    /*!
     * @brief Used to get values from configuration array
     * @param key : The key identifying wanted array of values
     * @param container : The container where the array of values will be pushed
     * @return container of values associated with key, of T type
     */
    template <typename T>
    bool getArray(const char *key, T &container)
    {
        return (getArray<T>(std::string(key), container));
    }

    /*!
     * @brief Used to set configuration values with String/String type.
     * @param key : The key indentifier to set
     * @param value : The formatted string value to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    void set(const std::string &key, const std::string value, bool serialize = false)
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

    /*!
     * @brief Used to set configuration values with any primitive type.
     * @param key : The key indentifier to set
     * @param value : The primitive-typed value to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template <typename T>
    void set(const std::string &key, const T &value, bool serialize = false)
    {
         std::ostringstream out;

        out << std::setprecision(DECIMAL_PRECISION) << value;
        set(key, out.str(), serialize);
    }

    /*!
     * @brief Used to set configuration values with an stl container.
     * @param key : The key indentifier to set
     * @param container : The container with values to fill in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template <typename T>
    void setArray(const std::string &key, const T &container, bool serialize = false)
    {
        std::string fValue;

        for (typename T::const_iterator it = container.begin(); it != container.end(); it++)
        {
            if (it != container.begin())
            {
                fValue += VALUE_FIELD_SEPARATOR;
            }
            fValue += std::to_string(*it);
        }
        set(key, fValue,serialize);
    }

    /*!
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
            _config.emplace(buffer.substr(0, separator), buffer.substr(separator + strlen(KEY_VALUE_SEPARATOR), buffer.size() - separator));
        }
        file.close();
        return (true);
    }


    /*!
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

    /* 
     * set specilization for following stl containers:
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
}

#endif /* !TINYCONF_HPP_ */