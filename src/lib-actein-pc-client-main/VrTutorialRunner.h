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
        std::future<void> RunAsync(bool wait);
        void Run(bool wait);
        void Stop();
        void WaitForTutorialStart();

    private:
        mutable std::mutex mSync;
        Settings & mSettings;
        std::wstring mTutorialRunCmd;
    };
}

#endif //VR_TUTORIAL_RUNNER_H__
