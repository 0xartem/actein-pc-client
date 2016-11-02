#include "ActeinService.h"
#include <thread>
#include <chrono>

namespace as
{
    ActeinService::ActeinService(boost_app::context & context)
        : mContext(context)
    {
    }

    int ActeinService::operator()()
    {
        //auto execPath = mContext.find<boost_app::path>()->executable_path().wstring();
        //auto args = mContext.find<boost_app::args>()->arg_vector();
        auto status = mContext.find<boost_app::status>();

        mContext.find<boost_app::wait_for_termination_request>()->wait();
        /*while (status->state() != boost_app::status::stopped)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }*/

        return 0;
    }

    bool ActeinService::stop()
    {
        return true;
    }

    bool ActeinService::resume()
    {
        return true;
    }

    bool ActeinService::pause()
    {
        return true;
    }
}