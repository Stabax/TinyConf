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
#include <memory>
// Tmp
#include <iostream>
// Stl Aggregate
#include <utility>
// Stl Containers
#include <vector>
#include <map>

/*! @brief This string represents the beginning of single line comment */
#define COMMENT_LINE_SEPARATOR  "#"
/*! @brief This string represents the beginning of a multi or single line comment block */
#define COMMENT_BLOCK_BEGIN     "/*"
/*! @brief This string represents the end of a multi or single line comment block */
#define COMMENT_BLOCK_END       "*/"
/*! @brief This is the string that braces a string to identify it */
#define STRING_IDENTIFIER       "\""
/*! @brief This is the string that braces a character to identify it */
#define CHAR_IDENTIFIER         "'"
/*! @brief This is the string between the key and the value in configuration file */
#define KEY_VALUE_SEPARATOR     "="
/*! @brief This is the string that separates multiple values in the field */
#define VALUE_FIELD_SEPARATOR   ":"
/*! @brief This is the number of digits that floats displays (including left-positioned digits) */
#define DECIMAL_PRECISION       10

/* Everything is defined within stb:: scope */
namespace stb {

namespace config {

    /*!
     * @class SNode
     * @brief Superclass holding templated node
     */
    template <typename T> struct Node; //Forward
    struct SNode
    {
        /*! @brief Enum defining possible identifiable types from config file */
        enum ValueType {
            None,
            Integral,
            Floating,
            Boolean,
            String,
            Char
        };

        /*! @brief Node default constructor for stl container ordering */
        SNode() { };
        /*! @brief Node default destructor for polymorphism */
        virtual ~SNode() = default;
        
        /*!
         * @brief Node constructor with value association
         * @param kType : The predicted type of value associated to key
         * @param kModified : True if the key is in an different state from file
         */
        SNode(ValueType kType, size_t kSize)
         : type(kType), size(kSize)
        {
            modified = false;
        }

        template <typename T>
        T getValue()
        {
			return (dynamic_cast<Node<T>*>(this)->value);
        }

        /*!
         * @brief get a ValueType based on template call
         * @return A ValueType corresponding to template argument type
         */
        template <typename T>
        static SNode::ValueType getValueType()
        {
			if (std::is_same<T, bool>::value) return (SNode::ValueType::Boolean);
            else if (std::is_integral<T>::value) return (SNode::ValueType::Integral);
            else if (std::is_floating_point<T>::value) return (SNode::ValueType::Floating);
            else if (std::is_same<T, char>::value) return (SNode::ValueType::Char);
            else if (std::is_same<T, char *>::value
                  || std::is_same<T, std::string>::value) return (SNode::ValueType::String);
        }

        bool modified;
		size_t size;
        ValueType type;
        std::shared_ptr<SNode> prev, next;
    };

    /*!
     * @class Node
     * @brief Represent a key/value association in memory
     */
    template <typename T>
    struct Node : public SNode
    {
        /*!
         * @brief Node default constructor for stl container ordering
         */
        Node() { };

        /*!
         * @brief Node constructor with value association
         * @param kValue : The value associated to key
         */
        Node(T kValue)
         : SNode(SNode::getValueType<T>(), sizeof(T)), value(kValue)
        {

        }

		/*!
		* @brief Node constructor from string with value association
		* @param kType : The wanted type of value
		* @param kValue : The value associated to key
		*/
		Node(ValueType kType, std::string kValue)
			: SNode(kType, kValue.length())
		{
			std::istringstream iss;

			if (type == ValueType::Integral
			 || type == ValueType::Floating)
			{
				iss.str(kValue);
				iss >> value;
			}
			else if (type == ValueType::Char)
			{
				value = kValue[0];
			}
		}

        T value;
    };

    /*
     * @brief Used to get string versions of values to serialize
     */
    template <>
    std::string SNode::getValue<std::string>()
    {
        std::ostringstream oss;

        if (type == ValueType::Integral)
        {
            if (size == sizeof(int16_t))
                oss << dynamic_cast<Node<int16_t>*>(this)->value;
            if (size == sizeof(int32_t))
                oss << dynamic_cast<Node<int32_t>*>(this)->value;
            if (size == sizeof(int64_t))
                oss << dynamic_cast<Node<int64_t>*>(this)->value;
            return (oss.str());
        }
        else if (type == ValueType::Floating)
        {
            if (size == sizeof(float))
                oss << dynamic_cast<Node<float>*>(this)->value;
            if (size == sizeof(double))
                oss << dynamic_cast<Node<double>*>(this)->value;
            return (oss.str());
        }
        else if (type == ValueType::String)
        {
            return (dynamic_cast<Node<std::string>*>(this)->value);
        }
    }

}

/*!
 * @class Config
 * @brief Main Config class: Defines the whole library
 */
class Config
{
public:
    /*! @brief Type used to represent associations in memory */
    typedef std::pair<std::string, std::shared_ptr<config::SNode>> association;
    /*! @brief Container used to store associations in memory */
    typedef std::map<std::string, std::shared_ptr<config::SNode>> associationMap;

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
            value = _config[key]->getValue<T>();
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
            //fill pair
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
            //fill array
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
    void setNode(const std::string key, const std::shared_ptr<config::SNode> value)
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
        setNode(key, std::make_shared<config::Node<T>>(value));
    }

    /*!
     * @brief Set configuration values with the contents of an std::pair.
     * @param key : The key indentifier to set
     * @param pair : The pair with values to fill in key field
     */
    template<typename Tx, typename Ty>
    void setPair(const std::string key, const std::pair<Tx, Ty> &pair)
    {
        /* Find a way to set pair */
    }

    /*!
     * @brief Set configuration values with the contents of any stl container implementing const_iterator.
     * @param key : The key indentifier to set
     * @param container : The container with values to fill in key field
     */
    template <typename T>
    void setArray(const std::string key, const T &container)
    {
        /* Find a way to set array */
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
                //_config.emplace(destKey, srcKey);
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
     * @param line : string to parse for comments
     * @param inside : true when inside a comment, false when not
     * @param remove : if true, comments will be removed from line
     * @return true when the buffer contains a valid key/value node
     */
    bool filterComments(std::string &line, bool &inside, bool remove = false)
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
        if (remove) line = buffer;
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

            if (buffer.compare(cursor, strlen(STRING_IDENTIFIER), STRING_IDENTIFIER) //Beginning of a string value, valid
             || buffer.compare(cursor, strlen(CHAR_IDENTIFIER), CHAR_IDENTIFIER)) //Beginning of a char value, valid
                return (sep);
            while (cursor < buffer.length()) //Check for numeric values
            {
                if (isdigit(buffer[cursor]) != 0 && buffer[cursor] != '.') //Not a number or decimal
                {
                    if (buffer.compare(cursor, strlen(VALUE_FIELD_SEPARATOR), VALUE_FIELD_SEPARATOR) == 0) //If value is not part of an array
                        return (getSeparator(buffer.substr(0, sep))); //The separator was a false positive, backtrack
                }
                sep++;
            }
            return (sep);
        }
        return (std::string::npos);
    }

    /*!
     * @brief Factory that creates a Node based on given predicted type
     * @param type : The predicted type of node
     * @return A Node built from the given predicted type
     */
    std::shared_ptr<config::SNode> createNodeFromString(config::SNode::ValueType type, const std::string &value)
    {
        switch (type)
        {
            case config::SNode::ValueType::Boolean:
                return (std::make_shared<config::Node<bool>>(type, value));
                break;
            case config::SNode::ValueType::Char:
                return (std::make_shared<config::Node<char>>(type, value));
                break;
            case config::SNode::ValueType::String:
                return (std::make_shared<config::Node<std::string>>(type, value));
                break;
            case config::SNode::ValueType::Integral:
                return (std::make_shared<config::Node<int>>(type, value));
                break;
            case config::SNode::ValueType::Floating:
                return (std::make_shared<config::Node<float>>(type, value));
                break;
            default:
                return (nullptr);
                break;
        }
    }

    /*!
     * @brief parse a buffer and extract a Node from it
     * @param buffer : The buffer to parse for Node
     * @return An association built from the value with its predicted type
     */
    std::shared_ptr<config::SNode> extractNode(const std::string &buffer)
    {
        size_t bov = 0, eov;
		config::SNode::ValueType type;
        bool array = false, eol = false, vfound;
        std::shared_ptr<config::SNode> node, pnode;

        while (!eol && bov < buffer.length())
        {
			type = config::SNode::ValueType::None;
            if (buffer.compare(bov, strlen(CHAR_IDENTIFIER), CHAR_IDENTIFIER) == 0) //Value is a char
            {
                type = config::SNode::ValueType::Char;
                bov = bov + strlen(CHAR_IDENTIFIER);
            }
            else if (buffer.compare(bov, strlen(STRING_IDENTIFIER), STRING_IDENTIFIER) == 0) //Value is a string
            {
                type = config::SNode::ValueType::String;
                bov = bov + strlen(STRING_IDENTIFIER);
            }
            else if (buffer.compare(bov, 4, "true") == 0 || buffer.compare(bov, 5, "false") == 0) //Value is bool
            {
                type = config::SNode::ValueType::Boolean;
            }
            //Beginning found. Now we need to predicate if not done already, and find eov
            vfound = false;
			eov = 0;
            while (eov < buffer.length() && !vfound)
            {
                if (type != config::SNode::ValueType::None) //Braced expression
                {
                    if (type == config::SNode::ValueType::Char && buffer.compare(eov, strlen(STRING_IDENTIFIER), STRING_IDENTIFIER) == 0) //End of Char
                    {
                        vfound = true;
                    }
                    else if (type == config::SNode::ValueType::String && buffer.compare(eov, strlen(STRING_IDENTIFIER), STRING_IDENTIFIER) == 0) //End of String
                    {
                        vfound = true;
                    }
                }
                if (buffer[eov] == '.' || buffer[eov] == ',') //Value is Floating
                {
                    type = config::SNode::ValueType::Floating;
                }
                if (buffer.compare(eov, strlen(VALUE_FIELD_SEPARATOR), VALUE_FIELD_SEPARATOR) == 0) //End of value, is an array
                {
                    array = true;
                    vfound = true;
                }
                else if (!isdigit(buffer[eov]) && buffer[eov] != '.' && buffer[eov] != ',') //End of any value
                {
                    if (type == config::SNode::ValueType::None) type = config::SNode::ValueType::Integral;
                    vfound = true;
                    eol = true;
                }
                eov++;
            }
            if (node == nullptr) //First item, create node
            {
                node = createNodeFromString(type, "VALUE");
            }
            //node.value = push!!
            if (array)
            {
                node->prev = pnode;
                pnode->next = node;
                pnode = node;
            }
			bov = eov;
        }
        return (node);
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
            if (filterComments(buffer[i], comment, true))
            {
                size_t separator = getSeparator(buffer[i]);
                if (separator == std::string::npos) continue;
                size_t bov = separator;
                while (bov > 0 && buffer[i][bov-1] != ' ') bov--;
                pair.first = buffer[i].substr(bov, separator - bov);
                pair.second = extractNode(buffer[i].substr(separator + strlen(KEY_VALUE_SEPARATOR), buffer[i].length() - separator));
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
            if (filterComments(buffer[i], comment))
            {
                size_t separator = getSeparator(buffer[i]);
                if (separator == std::string::npos) continue;
				size_t bov = separator;
				while (bov > 0 && buffer[i][bov - 1] != ' ') bov--;
				key = buffer[i].substr(bov, separator - bov);
                if (config.find(key) != config.end())
                {
                    size_t eov = separator + strlen(KEY_VALUE_SEPARATOR);
                    while (eov < buffer[i].length() && buffer[i][eov+1] != ' ') eov++;
                    eov -= separator + strlen(KEY_VALUE_SEPARATOR);
                    buffer[i].replace(separator + strlen(KEY_VALUE_SEPARATOR), eov, config[key]->getValue<std::string>());
                    config.erase(key);
                }
            }
        }
        for (associationMap::iterator it = config.begin(); it != config.end(); it++)
        {
            buffer.push_back(it->first + KEY_VALUE_SEPARATOR + it->second->getValue<std::string>());
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