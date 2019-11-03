#pragma once
#include <iomanip>

namespace GlibUtils
{
    template<typename realnumber>
    auto dtostr =  [](const realnumber val, const int &precision)->Glib::ustring 
    { 
        return Glib::ustring::format(std::fixed, std::setprecision(precision), val);
    };
}
