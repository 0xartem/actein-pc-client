#include <boost/application.hpp>
#include <boost/application/auto_handler.hpp>
#include <example/setup/windows/setup/service_setup.hpp>
#include "ActeinService.h"

int main(int argc, wchar_t ** argv)
{
    try
    {
        boost_app::context appContext;

        boost_app::auto_handler<as::ActeinService> app(appContext);

        appContext.insert<boost_app::path>(std::make_shared<boost_app::path>());
        appContext.insert<boost_app::args>(std::make_shared<boost_app::args>(argc, argv));

        return boost_app::launch<boost_app::server>(app, appContext);
    }
    catch (const boost::system::system_error & se)
    {
        std::cerr << se.what() << std::endl;
    }
}