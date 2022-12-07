#pragma once
#include <plog/Appenders/IAppender.h>
#include <plog/Util.h>
#include <plog/WinApi.h>
#include <iostream>
#include "api/logging.h"

namespace plog
{
    template<class Formatter>
    class LuaAppender : public IAppender
    {
    public:
        LuaAppender()
        {

        }

        virtual void write(const Record& record)
        {
            util::nstring str = Formatter::format(record);
            util::MutexLock lock(m_mutex);

            writestr(str);
        }


    protected:
        void writestr(const util::nstring& wstr)
        {
            std::string str;
#ifdef _WIN32
            std::size_t size;
            str.resize(wstr.length());
            wcstombs_s(&size, &str[0], str.size() + 1, wstr.c_str(), wstr.size());
#else
            str = wstr;
#endif

            cvedia::rt::api::logging::pushLogEntry(str);
        }

    private:

    protected:
        util::Mutex m_mutex;

    };
}
