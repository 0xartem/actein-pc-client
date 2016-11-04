#include <boost/application.hpp>
#include <boost/application/auto_handler.hpp>
#include "ActeinService.h"

int main(int argc, char ** argv)
{
        try
        {
            boost_app::context appContext;

            appContext.insert<boost_app::path>(std::make_shared<boost_app::path>());
            appContext.insert<boost_app::args>(std::make_shared<boost_app::args>(argc, argv));

            boost_app::auto_handler<as::ActeinService> app(appContext);
            return boost_app::launch<boost_app::common>(app, appContext);
        }
        catch (const boost::system::system_error & se)
        {
            std::cerr << se.what() << std::endl;
            return 1;
        }
}