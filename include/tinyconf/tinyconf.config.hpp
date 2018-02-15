#ifndef TINYCONF_CONFIG_HPP
#define TINYCONF_CONFIG_HPP

/*! * * * * * * * * * * * * * * * * * * * *
 * TinyConf Library
 * @version 0.1
 * @file tinyconf.hpp
 * @author Maxime 'Stalker2106' Martens
 * @brief Additional header for parser configuration
 * 
 * Any of the following values are modifiable without incidence in the parser behaviour
 * Be aware that any changes made unvalidate previously created configuration files
 * Separators (note the plural) represent "possible characters", where you can add custom values inside the define
 * Blocks and Separator values are string sequences to identify entities
 * The escape char should always be a single character.
 * * * * * * * * * * * * * * * * * * * * */

/*! @brief This string contains the sequence that escapes the next char */
#define CHARACTER_ESCAPE  '\\'
/*! @brief This string contains the characters that indicate a single line comment */
#define COMMENT_LINE_SEPARATORS  ";#"
/*! @brief This char represents the beginning of a comment block */
#define COMMENT_BLOCK_BEGIN     "/*"
/*! @brief This char represents the end of a comment block */
#define COMMENT_BLOCK_END       "*/"

/*! @brief This is the char that separates the section from the key in the field */
#define SECTION_FIELD_SEPARATOR   ":"
/*! @brief This char represents the beginning of a comment block */
#define SECTION_BLOCK_BEGIN     "["
/*! @brief This char represents the end of a comment block */
#define SECTION_BLOCK_END       "]"

/*! @brief This string contains characters that can brace strings for allowing use of forbidden chars */
#define STRING_IDENTIFIERS       "\"'"
/*! @brief This is the char between the key and the value in configuration file */
#define KEY_VALUE_SEPARATOR     "="
/*! @brief This is the char that separates multiple values in the field */
#define VALUE_FIELD_SEPARATOR   ":"
/*! @brief This is the number of digits that floats displays (including left-positioned digits) */
#define DECIMAL_PRECISION       100

#endif /* !TINYCONF_CONFIG_HPP */