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
#include <vector>
#include <map>

/*! @brief This string contains the sequence that escapes the next char */
#define CHARACTER_ESCAPE  '\\'
/*! @brief This string contains the characters that indicate a single line comment */
#define COMMENT_LINE_SEPARATORS  ";#"
/*! @brief This char represents the beginning of a comment block */
#define COMMENT_BLOCK_BEGIN     "/*"
/*! @brief This char represents the end of a comment block */
#define COMMENT_BLOCK_END       "*/"
/*! @brief This string contains characters that can brace strings for allowing use of forbidden chars */
#define STRING_IDENTIFIERS       "\"'"
/*! @brief This is the char between the key and the value in configuration file */
#define KEY_VALUE_SEPARATOR     "="
/*! @brief This is the char that separates multiple values in the field */
#define VALUE_FIELD_SEPARATOR   ":"
/*! @brief This is the number of digits that floats displays (including left-positioned digits) */
#define DECIMAL_PRECISION       100

/* Everything is defined within stb:: scope */
namespace stb {

/*!
 * @class Config
 * @brief Main Config class: Defines the whole library
 */
class Config
{
public:
    /*! @brief Type used to represent associations in memory */
    typedef std::pair<std::string, std::string> association;
    /*! @brief Container used to store associations in memory */
    typedef std::map<std::string, std::string> associationMap;

    /*! @brief Config empty constructor */
    Config() {}

    /*!
     * @brief Config standard constructor
     * @param path : The path where the file.cfg will reside
     */
    Config(const std::string &path, bool overwrite = false) : _path(path)
    {
        load();
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
     * @brief Set path of associated configuration file
     * @param path :  The path where the file.cfg will reside
     */
    void setPath(const std::string &path)
    {
        return (_path);
    }

    /*!
     * @brief Reload configuration from associated file
     */
    void reload()
    {
        relocate(_path);
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
    bool destroy()
    {
        return (destroy(_path));
    }

    /*!
     * @brief Removes a given configuration file from disk
     * @param path : The path to the configuration file to delete
     * @return true on success, false on failure
     */
    static bool destroy(const std::string &path)
    {
        if (remove(path.c_str()) != 0)
        {
            return (false); //Error on delete
        }
        return (true);
    }

    /*!
     * @brief Tests if a key exists in configuration
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
     * @brief Tests if a value equals a given param in configuration
     * @param key : The key to search for
     * @return true if equals, false if not
     */
    template <typename T>
    bool compare(const std::string key, const T &value)
    {
        if (_config.find(key) != _config.end())
        {
            if (_config[key] == value)
                return (true);
        }
        return (false);
    }

    //
    // GETTERS
    //

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
     * @brief Get bool values from configuration
     * @param key : The key identifying wanted value
     * @param value : The bool to set with value
     * @return true if found, false if failed
     */
    bool get(const std::string key, bool &value)
    {
        if (_config.find(key) != _config.end())
        {
            value = (_config[key] == "true" ? true : false);
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
     * @brief Used to get container-values from configuration
     * @param key : The key identifying wanted container of values
     * @param container : The container where the container of values will be pushed
	 * @return true on success, false on failure.
     */
    template <typename T>
    bool getContainer(const std::string key, T &container)
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
     * @brief Set configuration values with arithmetic types.
     * @param key : The key indentifier to set
     * @param value : The primitive-typed value to set in key field
     */
    template <typename T>
    void set(const std::string key, const T &value)
    {
        std::string sValue;
        if (std::is_arithmetic<T>::value)
        {
            std::ostringstream out;

            if (std::is_floating_point<T>::value)
            {
                out << std::setprecision(DECIMAL_PRECISION) << value;
            }
            else
            {
                out << value;
            }
            sValue = out.str();
        }
        else if (std::is_same<T, bool>::value)
        {
            sValue = (value ? "true" : "false");
        }
        else
        {
            sValue = value;
        }
        set(key, sValue);
    }

    /*!
     * @brief Set configuration values with arithmetic types.
     * @param key : The key indentifier to set
     * @param value : The primitive-typed value to set in key field
     */
    void set(const std::string key, const std::string &value)
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
     * @brief Set configuration values with bool types.
     * @param key : The key indentifier to set
     * @param value : The bool-typed value to set in key field
     */
    void set(const std::string key, const bool &value)
    {        
        set(key, (value ? "true" : "false"));
    }

    /*!
     * @brief Set configuration values with the contents of an std::pair.
     * @param key : The key indentifier to set
     * @param pair : The pair with values to fill in key field
     */
    template<typename Tx, typename Ty>
    void setPair(const std::string key, const std::pair<Tx, Ty> &pair)
    {
        std::string fValue;

        fValue += std::to_string(pair.first);
        fValue += VALUE_FIELD_SEPARATOR;
        fValue += std::to_string(pair.second);
        set(key, fValue);
    }

    /*!
     * @brief Set configuration values with the contents of any stl container implementing const_iterator.
     * @param key : The key indentifier to set
     * @param container : The container with values to fill in key field
     */
    template <typename T>
    void setContainer(const std::string key, const T &container)
    {
        std::string fValue;

        for (typename T::const_iterator it = container.cbegin(); it != container.cend(); it++)
        {
            if (it != container.begin())
            {
                fValue += VALUE_FIELD_SEPARATOR;
            }
            fValue += std::to_string(*it);
        }
        set(key, fValue);
    }

    //
    // MODIFIERS
    //

    /*!
     * @brief Used to rename configuration key
     * @param srcKey : The source key containing the value to copy
     * @param destKey : The destination key fill with source value
     */
    void move(const std::string srcKey, const std::string destKey)
    {
        if (_config.find(srcKey) != _config.end())
        {
           copy(srcKey, destKey);
           erase(srcKey);
        }
        else
        {
            throw (-1); //No source to move from !
        }
    }



    /*!
     * @brief Used to copy configuration value into another
     * @param srcKey : The source key containing the value to copy
     * @param destKey : The destination key fill with source value
     */
    void copy(const std::string srcKey, const std::string destKey)
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
     */
    void erase(const std::string key)
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
    // BASIC MECHANICS & PARSER
    //

    /*!
     * @brief Check for comments in a given string, and removes them if any
     * @param buffer : string to parse for comments
     * @return true when the buffer contains a valid key/value node
     */
    bool formatBuffer(std::string &buffer)
    {
        static bool inside = false;
        size_t begin = 0;

        for (size_t cursor = 0; cursor < buffer.length(); cursor++) //Parse line
        {
            if (inside) //If we are inside block
            {
                if (buffer.compare(cursor, strlen(COMMENT_BLOCK_END), COMMENT_BLOCK_END) == 0) //End found
                {
                    inside = false;
                    buffer.erase(begin, cursor + strlen(COMMENT_BLOCK_END));
                }
            }
            else //If not inside block
            {
                if (buffer.compare(cursor, strlen(COMMENT_BLOCK_BEGIN), COMMENT_BLOCK_BEGIN) == 0) //Block found
                {
                    inside = true;
                    begin = cursor;
                }
                else //check for line separator
                {
                    for (size_t i = 0; i < strlen(COMMENT_LINE_SEPARATORS); i++)
                    {
                        if (buffer[cursor] == COMMENT_LINE_SEPARATORS[i]) //separator found
                        {
                            buffer = buffer.substr(0, cursor);
                        }
                    }
                }
            }
        }
        return (!inside);
    }

    /*!
     * @brief Check for key and value in a given string, and returns the association
     * @param line : string to parse for key/value
     * @return pair of key and value
     */
    association parseBuffer(std::string &buffer)
    {
        association pair;
        size_t separator, begin;
        bool sepFound = false;
        
        for (size_t cursor = 0; cursor < buffer.length(); cursor++) //Parse line
        {
            while (buffer[cursor] == ' ') cursor++;
            for (size_t i = 0; i < strlen(STRING_IDENTIFIERS); i++) //check for string identifiers
            {
                if (buffer[cursor] == STRING_IDENTIFIERS[i] //identifier found
                && (cursor == 0 || (cursor > 0 && buffer[cursor-1] != CHARACTER_ESCAPE))) //check for non escaped sequence
                {
                    begin = cursor;
                    while (cursor < buffer.length()) //while not at the end
                    {
                        if (buffer[cursor] == STRING_IDENTIFIERS[i] //if we are on an identifier
                        && (cursor == 0 || (cursor > 0 && buffer[cursor-1] != CHARACTER_ESCAPE))) //check for non escaped sequence
                        {
                            if (!sepFound)
                            {
                                pair.first = buffer.substr(begin, cursor);
                            }
                            else
                            {
                                pair.second = buffer.substr(begin, cursor);
                                return (pair);
                            }
                        }
                        cursor++;
                    }
                }
            }
            if (buffer.compare(cursor, strlen(KEY_VALUE_SEPARATOR), KEY_VALUE_SEPARATOR) == 0 //sep found
            && (cursor == 0 || (cursor > 0 && buffer[cursor-1] != CHARACTER_ESCAPE))) //check for non escaped sequence
            {
                separator = cursor;
                sepFound = true;
            }
        }
        //If we end here, no identifiers were found
        pair.first = buffer.substr(0, separator);
        pair.second = buffer.substr(separator + strlen(KEY_VALUE_SEPARATOR), buffer.length() - separator + strlen(KEY_VALUE_SEPARATOR));
        return (pair);
    }

    /*!
     * @brief Load config stored in the associated file.
     * @return true on success, false on failure.
     */
    bool load()
    {
		std::vector<std::string> buffer = dump();
		size_t separator, bov, eov;
        association pair;

        for (size_t i = 0; i < buffer.size(); i++)
        {
            if (formatBuffer(buffer[i]))
            {
                pair = parseBuffer(buffer[i]);
                set(pair.first, pair.second);
            }
        }
        return (true);
    }

    /*!
     * @brief Dump current config file into a vector buffer.
     * @return A vector buffer containing a dump of the config file.
     */
    std::vector<std::string> dump() const
    {
        std::ifstream file(_path, std::ifstream::in);
        std::vector<std::string> buffer;
        std::string line;

        if (!file.good()) //No config, or could not open
        {
			return (buffer);
        }
        while (std::getline(file, line))
            buffer.push_back(line);
        file.close();
        return (buffer);
    }

    /*!
     * @brief Save current config state inside associated file.
     * @return true on success, false on failure.
     */
    bool save()
    {
        associationMap config = _config;
        std::vector<std::string> buffer = dump(), serialized = buffer;
        std::ofstream file(_path, std::ofstream::out | std::ofstream::trunc);
        association pair;

        if (!file.good())
        {
            return (false); //Couldnt open
        }
        for (size_t i = 0; i < buffer.size(); i++)
        {
            if (formatBuffer(serialized[i]))
            {
                pair = parseBuffer(serialized[i]);
                if (config.find(pair.first) != config.end())
                {
                    buffer[i].replace(buffer[i].find(pair.second), pair.second.length(), pair.second);
                    config.erase(pair.first);
                }
            }
        }
        for (associationMap::iterator it = config.begin(); it != config.end(); it++)
        {
            buffer.push_back(it->first + KEY_VALUE_SEPARATOR + it->second);
        }
        for (size_t i = 0; i < buffer.size(); i++)
        {
            file << buffer[i] << '\n';
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
     */
    void copyTo(const std::string key, Config &target)
    {
        if (exists(key))
        {
            target.set(key, _config[key]);
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
    void moveTo(const std::string key, Config &target)
    {
        try {
            copyTo(key, target);
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
    void append(const Config &source)
    {
        for (associationMap::const_iterator it = source._config.begin(); it != source._config.end(); it++)
        {
            set(it->first, it->second);
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
    associationMap _config;
    std::string _path;
};

}

#endif /* !TINYCONF_HPP_ */