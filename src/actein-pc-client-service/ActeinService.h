#ifndef ACTEIN_SERVICE_H__
#define ACTEIN_SERVICE_H__

#define BOOST_APPLICATION_FEATURE_NS_SELECT_BOOST

#include <vector>
#include <boost/application.hpp>

namespace boost_app = boost::application;

namespace as
{
    class ActeinService
    {
    public:
        // Default constructor is needed for boost_app::auto_handler
        ActeinService();
        explicit ActeinService(boost_app::context & context);

        int operator()();
        bool stop();
        bool resume();
        bool pause();

    private:
        boost_app::context * mContext;
    };
}

#endif //ACTEIN_SERVICE_H__
