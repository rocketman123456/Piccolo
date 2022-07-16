#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

//#define PICCOLO_PROFILE

namespace Piccolo
{
    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

    struct ProfileResult
    {
        std::string name;

        FloatingPointMicroseconds start;
        std::chrono::microseconds elapsed_time;
        std::thread::id           thread_id;
    };

    struct InstrumentationSession
    {
        std::string name;
    };

    class Instrumentor
    {
    public:
        Instrumentor(const Instrumentor&) = delete;
        Instrumentor(Instrumentor&&)      = delete;

        void BeginSession(const std::string& name, const std::string& filepath = "results.json");
        void EndSession();
        void WriteProfile(const ProfileResult& result);

        static Instrumentor& Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        Instrumentor() : m_current_session(nullptr) {}
        ~Instrumentor() { EndSession(); }

        void WriteHeader();
        void WriteFooter();

        // Note: you must already own lock on mutex_ before
        // calling InternalEndSession()
        void InternalEndSession();

    private:
        std::mutex              m_mutex;
        InstrumentationSession* m_current_session;
        std::ofstream           m_output_stream;
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char* name);
        ~InstrumentationTimer();

        void Stop();

    private:
        const char*                                        m_name;
        std::chrono::time_point<std::chrono::steady_clock> m_start_time;
        bool                                               m_stopped;
    };

    namespace InstrumentorUtils
    {
        template<size_t N>
        struct ChangeResult
        {
            char data[N];
        };

        template<size_t N, size_t K>
        constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
        {
            ChangeResult<N> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < N)
            {
                size_t matchIndex = 0;
                while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 &&
                       expr[srcIndex + matchIndex] == remove[matchIndex])
                    matchIndex++;
                if (matchIndex == K - 1)
                    srcIndex += matchIndex;
                result.data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
                srcIndex++;
            }
            return result;
        }
    } // namespace InstrumentorUtils
} // namespace Piccolo

#if defined(PICCOLO_PROFILE)
// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || \
    defined(__ghs__)
#define PICCOLO_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define PICCOLO_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define PICCOLO_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define PICCOLO_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define PICCOLO_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define PICCOLO_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define PICCOLO_FUNC_SIG __func__
#else
#define PICCOLO_FUNC_SIG "PICCOLO_FUNC_SIG unknown!"
#endif

#define PICCOLO_PROFILE_BEGIN_SESSION(name, filepath) ::Piccolo::Instrumentor::Get().BeginSession(name, filepath)
#define PICCOLO_PROFILE_END_SESSION() ::Piccolo::Instrumentor::Get().EndSession()
#define PICCOLO_PROFILE_SCOPE_LINE2(name, line) ::Piccolo::InstrumentationTimer timer##line(name)
#define PICCOLO_PROFILE_SCOPE_LINE(name, line) PICCOLO_PROFILE_SCOPE_LINE2(name, line)
#define PICCOLO_PROFILE_SCOPE(name) PICCOLO_PROFILE_SCOPE_LINE(name, __LINE__)
#define PICCOLO_PROFILE_FUNCTION() PICCOLO_PROFILE_SCOPE(PICCOLO_FUNC_SIG)
#else
#define PICCOLO_PROFILE_BEGIN_SESSION(name, filepath)
#define PICCOLO_PROFILE_END_SESSION()
#define PICCOLO_PROFILE_SCOPE(name)
#define PICCOLO_PROFILE_FUNCTION()
#endif
