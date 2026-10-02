//---------------------------------------------------------------------------

#ifndef msmLoggerH
#define msmLoggerH
//---------------------------------------------------------------------------

#include <source_location>

#include "fmtLog.h"

namespace msm
{
    #define logLocation() logd("[{}]",std::source_location::current().function_name())
    #define logLocationOffset(offset) logd("[{}]:{}",std::source_location::current().function_name(), offset)

/*    constexpr void __vectorcall logLocation()
    {
        logd("{}({}:{})[{}]",std::source_location::current().file_name(), std::source_location::current().line(), std::source_location::current().column(), std::source_location::current().function_name());
    }

    constexpr void __vectorcall logLocationOffset(const std::size_t offset)
    {
        logd("{}({}:{})[{}]:{}",std::source_location::current().file_name(), std::source_location::current().line(), std::source_location::current().column(), std::source_location::current().function_name(), offset);
    }
*/


} // namespace msm

#endif
