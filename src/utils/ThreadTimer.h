#ifndef THREAD_TIMER_H__
#define THREAD_TIMER_H__

#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace utils
{
    class ThreadTimer
    {
    public:
        typedef std::chrono::system_clock ClockType;
        typedef ClockType::time_point TimePointType;
        typedef std::chrono::seconds DurationType;

        enum class UsageType
        {
            TimePointUsage,
            DurationUsage
        };

    public:
        explicit ThreadTimer(
            const UsageType & usageType = UsageType::DurationUsage,
            bool isAutoReset = false,
            const std::function<void()> & handler = std::function<void()>()
            );
        ~ThreadTimer();

        void Start();
        void Stop();
        bool IsRunning() const;
        void SetHandler(std::function<void()> handler);

        void SetUsage(UsageType usageType);
        UsageType GetUsageType() const;

        void SetTimePoint(const TimePointType & when);
        TimePointType GetTimePoint() const;

        void SetDuration(const DurationType & duration);
        DurationType GetDuration() const;

        void SetAutoReset(bool value);
        bool IsAutoReset() const;

        void RethrowExceptionIfAny();

    private:
        void TimerThread();

        ThreadTimer(const ThreadTimer &) = delete;
        ThreadTimer & operator=(const ThreadTimer &) = delete;

    private:
        std::thread m_worker;
        mutable std::mutex m_sync;
        std::condition_variable m_conditionRunning;
        std::exception_ptr m_currentException;

        std::function<void()> m_handler;
        TimePointType m_runTimePoint;
        DurationType m_runPeriod;

        bool m_isRunning;
        bool m_isAutoReset;
        UsageType m_usageType;
    };
}

#endif //THREAD_TIMER_H__