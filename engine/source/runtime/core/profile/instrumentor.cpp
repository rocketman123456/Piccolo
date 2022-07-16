#include "runtime/core/profile/instrumentor.h"
#include "runtime/core/base/macro.h"

namespace Piccolo
{
    void Instrumentor::BeginSession(const std::string& name, const std::string& filepath)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_current_session)
        {
            // If there is already a current session, then close it before beginning new one.
            // Subsequent profiling output meant for the original session will end up in the
            // newly opened session instead.  That's better than having badly formatted
            // profiling output.
            LOG_ERROR(
                "Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, m_current_session->name);
            InternalEndSession();
        }
        m_output_stream.open(filepath);

        if (m_output_stream.is_open())
        {
            m_current_session = new InstrumentationSession({name});
            WriteHeader();
        }
        else
        {
            LOG_ERROR("Instrumentor could not open results file '{0}'.", filepath);
        }
    }

    void Instrumentor::EndSession()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        InternalEndSession();
    }

    void Instrumentor::WriteProfile(const ProfileResult& result)
    {
        std::stringstream json;

        json << std::setprecision(3) << std::fixed;
        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << (result.elapsed_time.count()) << ',';
        json << "\"name\":\"" << result.name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":\"" << result.thread_id << "\",";
        json << "\"ts\":" << result.start.count();
        json << "}";

        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_current_session)
        {
            m_output_stream << json.str();
            m_output_stream.flush();
        }
    }

    void Instrumentor::WriteHeader()
    {
        m_output_stream << "{\"otherData\": {},\"traceEvents\":[{}";
        m_output_stream.flush();
    }

    void Instrumentor::WriteFooter()
    {
        m_output_stream << "]}";
        m_output_stream.flush();
    }

    void Instrumentor::InternalEndSession()
    {
        if (m_current_session)
        {
            WriteFooter();
            m_output_stream.close();
            delete m_current_session;
            m_current_session = nullptr;
        }
    }

    InstrumentationTimer::InstrumentationTimer(const char* name) : m_name(name), m_stopped(false)
    {
        m_start_time = std::chrono::steady_clock::now();
    }

    InstrumentationTimer::~InstrumentationTimer()
    {
        if (!m_stopped)
            Stop();
    }

    void InstrumentationTimer::Stop()
    {
        auto endTimepoint = std::chrono::steady_clock::now();
        auto highResStart = FloatingPointMicroseconds {m_start_time.time_since_epoch()};
        auto elapsedTime  = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch() -
                           std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time).time_since_epoch();

        Instrumentor::Get().WriteProfile({m_name, highResStart, elapsedTime, std::this_thread::get_id()});

        m_stopped = true;
    }
} // namespace Piccolo
