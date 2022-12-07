#pragma once
#include <plog/Record.h>
#include <plog/Util.h>
#include <iomanip>

#include "interface/instance.h"
#include "internal/threadmanager.h"

namespace plog
{
    template<bool useUtcTime>
    class TxtFormatterImpl
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            tm t;
            useUtcTime ? util::gmtime_s(&t, &record.getTime().time) : util::localtime_s(&t, &record.getTime().time);

            std::string context = "Global";

            auto inst_ = cvedia::rt::internal::ThreadManager::getActiveInstance();
            if (inst_)
            {
                context = inst_.value()->getName();
            }

            util::nostringstream ss;
            ss << t.tm_year + 1900 << "-" << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mon + 1 << PLOG_NSTR("-") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_mday << PLOG_NSTR(" ");
            ss << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_hour << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_min << PLOG_NSTR(":") << std::setfill(PLOG_NSTR('0')) << std::setw(2) << t.tm_sec << PLOG_NSTR(".") << std::setfill(PLOG_NSTR('0')) << std::setw(3) << static_cast<int> (record.getTime().millitm) << PLOG_NSTR(" ");
            ss << std::setfill(PLOG_NSTR(' ')) << std::setw(5) << std::left << severityToString(record.getSeverity()) << PLOG_NSTR(" ");
//            ss << PLOG_NSTR("[") << record.getTid() << PLOG_NSTR("] ");
            ss << PLOG_NSTR("[") << record.getFile() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR("] ");
            ss << PLOG_NSTR("[") << context.c_str() << PLOG_NSTR("] ");
            ss << record.getMessage() << PLOG_NSTR("\n");

            return ss.str();
        }
    };

    class TxtFormatter : public TxtFormatterImpl<false> {};
    class TxtFormatterUtcTime : public TxtFormatterImpl<true> {};
}
