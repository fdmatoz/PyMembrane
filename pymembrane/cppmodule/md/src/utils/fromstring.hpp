#ifndef __fromstring_hpp__
#define __fromstring_hpp__

#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <algorithm> 
#include <cctype>


namespace util
{

    /**
 * @brief to_string_scientific converts any number to scientific notation
 * 
 * @tparam T 
 * @param a_value value to be converted to
 * @return std::string string scientific notation formated
 */
    template <typename T>
    std::string to_string_scientific(const T a_value)
    {
        std::ostringstream out;
        out << std::scientific << a_value;
        return out.str();
    }

    template <typename T>
    std::string to_string_with_precision(const T a_value, const int n = 8)
    {
        //https://stackoverflow.com/questions/16605967/set-precision-of-stdto-string-when-converting-floating-point-values
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return out.str();
    }

    /**
 * @brief to_string_scientific converts any number to scientific notation
 * 
 * @tparam T 
 * @param a_value value to be converted to
 * @return std::string string scientific notation formated
 */
    template <typename T>
    std::string to_string(const T a_value)
    {
        std::ostringstream out;
        out << a_value;
        return out.str();
    }

    template <typename T>
    std::string to_string_vec(const std::vector<T> a_value)
    {
        std::ostringstream out;
        for (int i = 0; i < a_value.size(); ++i)
            out << a_value[i] << " ";
        return out.str();
    }

    inline int from_string_int(const std::string &string_val)
    {
        return (std::stoi(string_val));
    }

    inline double from_string_double(const std::string &string_val)
    {
        return (std::stod(string_val));
    }

    inline unsigned int from_string_unsigned(const std::string &string_val)
    {
        return (std::stoi(string_val));
    }

    inline bool from_string_bool(const std::string &string_val)
    {
        if (string_val.compare("true") == 0)
            return true;
        else if (string_val.compare("false") == 0)
            return false;
        else if (string_val.compare("True") == 0)
            return true;
        else if (string_val.compare("False") == 0)
            return false;
        else
            return false;
    }
} // namespace util

#endif