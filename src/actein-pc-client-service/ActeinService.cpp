#include "ActeinService.h"

namespace as
{
    ActeinService::ActeinService()
        : mContext(nullptr)
    {
    }

    ActeinService::ActeinService(boost_app::context & context)
        : mContext(&context)
    {
    }

    int ActeinService::operator()()
    {
        std::vector<std::wstring> args = mContext->find<boost_app::args>()->arg_vector();

        mContext->find<boost_app::wait_for_termination_request>()->wait();
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