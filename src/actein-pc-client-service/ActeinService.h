#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#include <vector>
#include <boost/application.hpp>

namespace boost_app = boost::application;

namespace as
{
    class ActeinService
    {
    public:
        ActeinService(boost_app::context & context);

        int operator()();
        bool stop();
        bool resume();
        bool pause();

    private:
        boost_app::context & mContext;
    };
}

#endif //ACTEIN_SERVICE_H__
