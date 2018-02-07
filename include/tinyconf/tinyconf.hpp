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

/*! @brief This char represents the beginning of single line comment */
#define COMMENT_LINE_SEPARATOR  "#"
/*! @brief This char represents the beginning of a multi or single line comment block */
#define COMMENT_BLOCK_BEGIN     "/*"
/*! @brief This char represents the end of a multi or single line comment block */
#define COMMENT_BLOCK_END       "*/"
/*! @brief This is the char between the key and the value in configuration file */
#define STRING_IDENTIFIER       "\""
/*! @brief This is the char between the key and the value in configuration file */
#define KEY_VALUE_SEPARATOR     "="
/*! @brief This is the char that separates multiple values in the field */
#define VALUE_FIELD_SEPARATOR   ":"
/*! @brief This is the number of digits that floats displays (including left-positioned digits) */
#define DECIMAL_PRECISION       10

/* Everything is defined within stb:: scope */
namespace stb {

/*!
 * @class Config
 * @brief Main Config class: Defines the whole library
 */
class Config
{
public:

    /*!
     * @class Node
     * @brief Superclass holding templated node
     */
    struct SNode
    {
        /*! @brief Enum defining possible identifiable types from config file */
        enum ValueType {
            Arithmetic,
            Boolean,
            String
        };

        /*! @brief Node default constructor for stl container ordering */
        SNode() { };
        
        /*!
         * @brief Node constructor with value association
         * @param kType : The predicted type of value associated to key
         */
        SNode(ValueType kType)
         : type(kType)
        {

        }

        /* Prepare for template
        template <typename T>
        T value() { return (dynamic_cast<Node<T>>(this)->value); }
        */

        ValueType type;
    };

    /*!
     * @class Node
     * @brief Represent a key/value association in memory
     */
    struct Node : public SNode
    {
        /*!
         * @brief Node default constructor for stl container ordering
         */
        Node() { };

        /*!
         * @brief Node constructor with value association
         * @param kValue : The string holding value associated to key
         * @param kType : The predicted type of value associated to key
         */
        Node(const std::string &kValue, ValueType kType = String)
         : value(kValue), SNode(kType)
        {

        }

        std::string value;
    };

    /*! @brief Type used to represent associations in memory */
    typedef std::pair<std::string, Node> association;
    /*! @brief Container used to store associations in memory */
    typedef std::map<std::string, Node> associationMap;

    /*!
     * @brief Config default constructor
     * @param path : The path where the file.cfg will reside
     */
    Config(const std::string &path) : _path(path)
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
            iss.str(_config[key].value);
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
            strcpy(value, _config[key].value.c_str());
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
            value = _config[key].value;
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
            size_t sep = _config[key].value.find(VALUE_FIELD_SEPARATOR);
            if (sep != std::string::npos)
            {
                std::string buffer = _config[key].value;
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
            std::string buffer = _config[key].value;

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
     */
    void set(const std::string key, const Node &value)
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
     */
    template <typename T>
    void set(const std::string key, const T &value)
    {
         std::ostringstream out;

        out << std::setprecision(DECIMAL_PRECISION) << value;
        set(key, Node(out.str(), Node::ValueType::String));
    }

    /*!
     * @brief Set configuration values with arithmetic types.
     * @param key : The key indentifier to set
     * @param value : The primitive-typed value to set in key field
     */
    void set(const std::string key, const std::string &value)
    {
        set(key, Node(value, Node::ValueType::String));
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
        set(key, Node(fValue, Node::ValueType::String));
    }

    /*!
     * @brief Set configuration values with the contents of any stl container implementing const_iterator.
     * @param key : The key indentifier to set
     * @param container : The container with values to fill in key field
     */
    template <typename T>
    void setArray(const std::string key, const T &container)
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
        set(key, Node(fValue, Node::ValueType::String));
    }

    //
    // MODIFIERS
    //

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
     * @param remove : true when comment should be removed from buffer, false when not
     * @param inside : true when inside a comment, false when not
     * @return true when the buffer contains a valid key/value node
     */
    bool filterComments(std::string &line, bool remove, bool &inside)
    {
		std::string buffer = line;
        size_t blocks, blocke;

        if (inside)
        {
            if ((blocke = buffer.find(COMMENT_BLOCK_END)) != std::string::npos) //Already inside, search for ending
            {
				buffer.erase(0, blocke + strlen(COMMENT_BLOCK_END)); //Removes comment from buffer
				inside = false;
            }
            else
            {
                return (false); //Comment does not end in this buffer, don't treat
            }
        }
        while ((blocks = buffer.find(COMMENT_BLOCK_BEGIN)) != std::string::npos) //Search for block comments to remove
        {
            if ((blocke = buffer.find(COMMENT_BLOCK_END)) != std::string::npos && blocke > blocks) //It ends inside, and after opening
            {
                buffer.erase(blocks, blocke + strlen(COMMENT_BLOCK_BEGIN) + strlen(COMMENT_BLOCK_END) - blocks); //Removes comment from buffer
				inside = false;
            }
			else
			{
				inside = true;
				return (true); //EOL inside comment block
			}
        }        
        while ((blocks = buffer.find(COMMENT_LINE_SEPARATOR)) != std::string::npos) //There is a line comment
        {
            buffer = buffer.substr(0, blocks); //Removes comment from buffer
        }
		if (remove)
			line = buffer;
        return (true); //Comments were removed from buffer
    }

    /*!
     * @brief Get separator position between key and value in buffer
     * @param buffer : string to parse for separator
     * @return position of first char of separator in buffer, -1 if no separator found
     */
    size_t getSeparator(const std::string &buffer)
    {
        size_t sep;

        if ((sep = buffer.find_last_of(KEY_VALUE_SEPARATOR)) != std::string::npos)
        {
            size_t cursor = sep + strlen(KEY_VALUE_SEPARATOR);

            if (STRING_IDENTIFIER != buffer.substr(cursor, strlen(STRING_IDENTIFIER))) //Beginning of a string value, valid
                return (sep);
            while (cursor < buffer.length()) //Check for numeric values
            {
                if (isdigit(buffer[cursor]) != 0 && buffer[cursor] != '.') //Not a number or decimal
                {
                    if (VALUE_FIELD_SEPARATOR != buffer.substr(cursor, strlen(VALUE_FIELD_SEPARATOR))) //If value is not part of an array
                        return (getSeparator(buffer.substr(0, sep))); //The separator was a false positive, backtrack
                }
                sep++;
            }
            return (sep);
        }
        return (std::string::npos);
    }

    /*!
     * @brief Load config stored in the associated file.
     * @return true on success, false on failure.
     */
    association extractNode(const std::string &buffer)
    {

    }

    /*!
     * @brief Load config stored in the associated file.
     * @return true on success, false on failure.
     */
    bool load()
    {
		std::vector<std::string> buffer = dump();
        association pair;
        bool comment = false;

        for (size_t i = 0; i < buffer.size(); i++)
        {
            if (filterComments(buffer[i], true, comment))
            {
                size_t separator = getSeparator(buffer[i]);
                if (separator == std::string::npos) continue;
                size_t bov = separator;
                while (bov > 0 && buffer[i][bov-1] != ' ') bov--;
                pair.first = buffer[i].substr(bov, separator - bov);
                pair.second = Node(buffer[i].substr(separator + strlen(KEY_VALUE_SEPARATOR), buffer[i].length() - separator), Node::ValueType::String);
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
        std::vector<std::string> serialized, buffer = dump();
        std::ofstream file(_path, std::ofstream::out | std::ofstream::trunc);
        std::string key;
        bool comment = false;

        if (!file.good())
        {
            return (false); //Couldnt open
        }
        for (size_t i = 0; i < buffer.size(); i++)
        {
            if (filterComments(buffer[i], false, comment))
            {
                size_t separator = getSeparator(buffer[i]);
                if (separator == std::string::npos) continue;
                key = buffer[i].substr(0, separator);
                if (config.find(key) != config.end())
                {
                    size_t eov = separator + strlen(KEY_VALUE_SEPARATOR);
                    while (eov < buffer[i].length() && buffer[i][eov] != ' ') eov++;
                    eov -= separator + strlen(KEY_VALUE_SEPARATOR);
                    buffer[i].replace(separator + strlen(KEY_VALUE_SEPARATOR), eov, config[key].value);
                    config.erase(key);
                }
            }
        }
        for (associationMap::iterator it = config.begin(); it != config.end(); it++)
        {
            buffer.push_back(it->first + KEY_VALUE_SEPARATOR + it->second.value);
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