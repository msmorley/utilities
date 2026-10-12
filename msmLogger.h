//---------------------------------------------------------------------------

#ifndef msmLoggerH
#define msmLoggerH
//---------------------------------------------------------------------------

#include <source_location>
#include <boost/uuid/uuid_io.hpp>

#include "fmtLog.h"

namespace msm
{
    #define logLocation() logd("[{}]",std::source_location::current().function_name())
    #define logLocationOffset(offset) logd("[{}]:{}",std::source_location::current().function_name(), offset)
    #define logLocationOffsetValue(offset, value) logd("[{}]:{} - {}",std::source_location::current().function_name(), offset, value)

} // namespace msm

#endif
