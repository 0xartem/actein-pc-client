#ifndef VR_TUTORIAL_RUNNER_H__
#define VR_TUTORIAL_RUNNER_H__

#include <string>
#include <future>

namespace actein
{
    class Settings;

    class VrTutorialRunner
    {
    public:
        explicit VrTutorialRunner(Settings & settings);
        std::future<void> RunAsync();
        void Run();
        void Stop();
        void WaitForTutorialStart(int waitTimeout = MAX_START_WAIT_TIME);

    private:
        mutable std::mutex mSync;
        Settings & mSettings;
        std::wstring mTutorialRunCmd;
        
        static const int MAX_START_WAIT_TIME = 8000;
        static const int WAIT_STEP = 1000;
    };
}

#endif //VR_TUTORIAL_RUNNER_H__
