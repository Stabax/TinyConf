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
#include <cstdio>
// Tmp
#include <iostream>
// Stl Aggregate
#include <utility>
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

/*! @brief This char represents beginning of comment lines */
#define COMMENT_LINE_SEPARATOR  "#"
/*! @brief This is the char sequence between the key and the value in configuration file */
#define KEY_VALUE_SEPARATOR     ":="
/*! @brief This is the char sequence that separates multiple values in the field */
#define VALUE_FIELD_SEPARATOR   ":=:"
/*! @brief This is the number of digits that floats displays (including left-positioned digits) */
#define DECIMAL_PRECISION       10

/* Everything is defined within stb:: scope */
namespace stb {

/*!
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

    ~Config()
    {

    }

   /*!
     * @brief Get path of associated configuration file
     * @return String containing the path of the current associated cfg file
     */
    std::string getPath()
    {
        return (_path);
    }

    /*!
     * @brief Reset object and load another configuration file
     * @param path : The path to the configuration file to relocate to
     */
    void relocate(const std::string &path)
    {
        _path = path;
        _config.clear();
        load();
    }

    /*!
     * @brief Removes associated configuration file from disk
     * @return true on success, false on failure
     */
    bool deleteFile()
    {
        return (deleteFile(_path));
    }

    /*!
     * @brief Removes a given configuration file from disk
     * @param path : The path to the configuration file to delete
     * @return true on success, false on failure
     */
    static bool deleteFile(const std::string &path)
    {
        if (remove(path.c_str()) != 0)
        {
            return (false); //Error on delete
        }
        return (true);
    }

    //
    // GETTERS
    //

    /*!
     * @brief Test if a key exists in configuration
     * @param key : The key to search for
     * @return true if found, false if failed
     */
    bool exists(const std::string key)
    {
        if (_config.find(key) != _config.end())
        {
            return (true);
        }
        return (false);
    }

    /*!
     * @brief Get arithmetic values from configuration
     * @param key : The key identifying wanted value
     * @param value : The arithmetic-typed variable to set with value
     * @return true if found, false if failed
     */
    template <typename T>
    bool get(const std::string key, T &value)
    {
        if (_config.find(key) != _config.end())
        {
            std::istringstream iss;
            iss.str(_config[key]);
            iss >> value;
            return (true);
        }
        return (false);
    }

    /*!
     * @brief Get C-style string values from configuration
     * @param key : The key identifying wanted value
     * @param value : The char array to set with value
     * @return true if found, false if failed
     */
    bool get(const std::string key, char *value)
    {
        if (_config.find(key) != _config.end())
        {
            strcpy(value, _config[key].c_str());
            return (true);
        }
        return (false);
    }

    /*!
     * @brief Get string values from configuration
     * @param key : The key identifying wanted value
     * @param value : The string to set with value
     * @return true if found, false if failed
     */
    bool get(const std::string key, std::string &value)
    {
        if (_config.find(key) != _config.end())
        {
            value = _config[key];
            return (true);
        }
        return (false);
    }

    /*!
     * @brief Get pair values from configuration
     * @param key : The key identifying wanted value
     * @param pair : The pair to fill with values
     * @return true if found, false if failed
     */
    template<typename Tx, typename Ty>
    bool getPair(const std::string key,  std::pair<Tx, Ty> &pair)
    {
        if (_config.find(key) != _config.end())
        {
            size_t sep = _config[key].find(VALUE_FIELD_SEPARATOR);
            if (sep != std::string::npos)
            {
                std::string buffer = _config[key];
                std::istringstream iss;

                iss.str(buffer.substr(0, sep));
                iss >> pair.first;
                iss.clear();
                iss.str(buffer.substr(sep + strlen(VALUE_FIELD_SEPARATOR),
                                            buffer.size() - sep + strlen(VALUE_FIELD_SEPARATOR)));
                iss >> pair.second;
            }
            return (true);
        }
        return (false);
    }

    /*!
     * @brief Used to get values from configuration array
     * @param key : The key identifying wanted array of values
     * @param container : The container where the array of values will be pushed
	 * @return true on success, false on failure.
     */
    template <typename T>
    bool getArray(const std::string key, T &container)
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
		    return (true);
        }
        return (false);
    }


    //
    // SETTERS
    //

    /*!
     * @brief Set configuration values with string type.
     * @param key : The key indentifier to set
     * @param value : The formatted string value to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    void set(const std::string key, const std::string value, bool serialize = false)
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
     * @brief Set configuration values with arithmetic types.
     * @param key : The key indentifier to set
     * @param value : The primitive-typed value to set in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template <typename T>
    void set(const std::string key, const T &value, bool serialize = false)
    {
         std::ostringstream out;

        out << std::setprecision(DECIMAL_PRECISION) << value;
        set(key, out.str(), serialize);
    }

    /*!
     * @brief Set configuration values with the contents of an std::pair.
     * @param key : The key indentifier to set
     * @param pair : The pair with values to fill in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template<typename Tx, typename Ty>
    void setPair(const std::string key, const std::pair<Tx, Ty> &pair, bool serialize = false)
    {
        std::string fValue;
        fValue += std::to_string(pair.first);
        fValue += VALUE_FIELD_SEPARATOR;
        fValue += std::to_string(pair.second);
        set(key, fValue, serialize);
    }

    /*!
     * @brief Set configuration values with the contents of any stl container implementing const_iterator.
     * @param key : The key indentifier to set
     * @param container : The container with values to fill in key field
     * @param serialize : Set this to true to save the changes right away to file
     */
    template <typename T>
    void setArray(const std::string key, const T &container, bool serialize = false)
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
        set(key, fValue, serialize);
    }

    //
    // MODIFIERS
    //

    /*!
     * @brief Used to copy configuration value into another
     * @param srcKey : The source key containing the value to copy
     * @param destKey : The destination key fill with source value
     * @param serialize : Set this to true to save the changes right away to file
     */
    void copy(const std::string srcKey, const std::string destKey, bool serialize = false)
    {
        if (_config.find(srcKey) != _config.end())
        {
            if (_config.find(destKey) != _config.end())
            {
                _config[destKey] = _config[srcKey];
            }
            else
            {
                _config.emplace(destKey, srcKey);
            }
        }
        else
        {
            throw (-1); //No source to copy from !
        }
    }

    /*!
     * @brief Erase a key from configuration
     * @param key : The key to erase
     * @param serialize : Set this to true to save the changes right away to file
     */
    void erase(const std::string key, bool serialize = false)
    {
        if (_config.find(key) != _config.end())
        {
            _config.erase(key);
        }
        else
        {
            throw (-1); //No key to erase !
        }
    }

    //
    // BASIC MECHANICS
    //

    /*!
     * @brief Load config stored in the associated file.
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
     * @brief Save current config state inside associated file.
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

    //
    // INTEROPERABILITY
    //

    /*!
     * @brief Copies a given key to another configuration
     * @param key : The key to copy
     * @param target : The target configuration to copy to
     * @param serialize : Set this to true to save the changes right away to file
     */
    void copyTo(const std::string key, Config &target, bool serialize = false)
    {
        if (exists(key))
        {
            target.set(key, _config[key], serialize);
        }
        else
        {
            throw (-1); //No key to copy !
        }
    }

    /*!
     * @brief Moves a given key to another configuration (removes key from caller)
     * @param key : The key to move
     * @param target : The target configuration to move to
     */
    void moveTo(const std::string key, Config &target, bool serialize = false)
    {
        try {
            copyTo(key, target, serialize);
        }
        catch(...)
        {
            return; //Failed! do not erase
        }
        erase(key);
    }

    /*!
     * @brief Append the target configuration to the caller
     * @param source : The configuration to copy keys from
     */
    void append(const Config &source, bool serialize = false)
    {
        for (std::map<std::string, std::string>::const_iterator it = source._config.begin(); it != source._config.end(); it++)
        {
            set(it->first, it->second, serialize);
        }
    }

    /*!
     * @brief Append the configuration file to the caller
     * @param path : The path to the configuration file to copy keys from
     */
    void append(const std::string &path)
    {
        Config source(path);

        append(source);
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