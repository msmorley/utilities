//---------------------------------------------------------------------------

#ifndef msmLoggerH
#define msmLoggerH
//---------------------------------------------------------------------------

#include <source_location>

#include <boost/uuid/uuid_io.hpp>
#include <boost/nowide/iostream.hpp>
#include <boost/nowide/fstream.hpp>
#include <boost/nowide/convert.hpp>

#include <print>

#include "fmtLog.h"

namespace msm
{
    extern "C" boost::nowide::ofstream loadLogger;
   	extern "C" boost::nowide::ofstream saveLogger;
}

    template <class T>
    void __vectorcall logLoadLocationOffsetValue(const std::size_t offset, const T& value)
    {
		::msm::loadLogger << fmt::format("[{}]:{} - {}\n", std::source_location::current().function_name(), offset, value);
        ::msm::loadLogger.flush();
	}

namespace msm
{
    #define logSaveLocation() ::msm::saveLogger << fmt::format("[{}]\n", std::source_location::current().function_name())
    #define logSaveLocationOffset(offset) ::msm::saveLogger << fmt::format("[{}]:{}\n", std::source_location::current().function_name(), offset)
    #define logSaveLocationOffsetValue(offset, value) ::msm::saveLogger << fmt::format("[{}]:{} - {}\n", std::source_location::current().function_name(), offset, value)

    #define logLoadLocation() ::msm::loadLogger << fmt::format("[{}]\n", std::source_location::current().function_name()); ::msm::loadLogger.flush()
    #define logLoadLocationOffset(offset) ::msm::loadLogger << fmt::format("[{}]:{}\n", std::source_location::current().function_name(), offset); ::msm::loadLogger.flush()
    //#define logLoadLocationOffsetValue(offset, value) ::msm::loadLogger << fmt::format("[{}]:{} - {}\n", std::source_location::current().function_name(), offset, value); ::msm::loadLogger.flush()


/*
    #define logSaveLocation() std::print(::msm::saveLogger, "[{}]\n", std::source_location::current().function_name())
    #define logSaveLocationOffset(offset) std::print(::msm::saveLogger, "[{}]:{}\n", std::source_location::current().function_name(), offset)
    #define logSaveLocationOffsetValue(offset, value) std::print(::msm::saveLogger, "[{}]:{} - {}", std::source_location::current().function_name(), offset, value)

    #define logLoadLocation() std::print(::msm::loadLogger, "[{}]", std::source_location::current().function_name())
    #define logLoadLocationOffset(offset) std::print(::msm::loadLogger, "[{}]:{}", std::source_location::current().function_name(), offset)
    #define logLoadLocationOffsetValue(offset, value) std::print(::msm::loadLogger, "[{}]:{} - {}", std::source_location::current().function_name(), offset, value)
*/

//    #define logLocation() logd("[{}]",std::source_location::current().function_name())
//    #define logLocationOffset(offset) logd("[{}]:{}",std::source_location::current().function_name(), offset)
//    #define logLocationOffsetValue(offset, value) logd("[{}]:{} - {}",std::source_location::current().function_name(), offset, value)

} // namespace msm

#endif
