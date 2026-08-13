//
// Created by nechi on 07.03.2022.
//

#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include <cmath>
#include <utility>
#include <vector>
#include "../include//db-types.h"


enum EBoolTypes{
    EBT_DIGIT,
    EBT_LOWSTR,
    EBT_UPSTR
};

class CFunctions{

private:
    static Tuint64 StringToDigits(const Tstring & str);
    static Tstring DigitsToString(const Tuint64 & dig);


public:

    static Tint32 ToInt32(const Tstring & str);
    static double ToDouble(const Tstring & str, const Tsize & precision = 4);
    static long double ToLongDouble(const Tstring & str, const Tsize & precision = 4);
    static bool   ToBool(const Tstring & str);


    //static Tstring ToString(const Tint16 & value);
    static Tstring ToString(const Tint32 & value);
    static Tstring ToString(const double & value, const Tsize & prec = 4, const bool & ispoint = true);
    static Tstring ToString(const long double & value, const Tsize & prec = 4, const bool & ispoint = true);
    static Tstring ToString(const unsigned & value);
    static Tstring ToString(const long & value);
    static Tstring ToString(const unsigned long & value);
#ifdef Windows
    static Tstring ToString(const Tsize & value);
    static Tstring ToString(const time_t & value);
#endif
    static Tstring ToString(const bool & value, const EBoolTypes & type = EBT_DIGIT);

    static Tint32 fromChar4(const char *raw_data);

    static Tstring get_file_extension(const Tstring &file_name);


    /**
     * @brief trims string from start
     */
    static Tstring &ltrim(Tstring &s);

    /**
     * @brief trims string from end
     */
    static Tstring &rtrim(Tstring &s);
    /** @brief trim from both ends*/
    static Tstring &trim(Tstring &s);

    static bool is_integer(const Tstring &text);
    static bool is_float(const Tstring &text);

    //static bool is_floats_equal(const long double &val1, const long double &val2);

    [[nodiscard]]static Tstring  md5_hash(const Tstring &message);

    //static Tstring trim(const Tstring &str, const Tstring &whitespace);
};




#endif //FUNCTIONS_H
