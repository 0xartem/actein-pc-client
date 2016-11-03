#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>
#include <mqtt/exception.h>
#include <mqtt/iaction_listener.h>
#include <Connection.h>
#include <IVrEventsManager.h>
#include <VrEventsException.h>
#include <ConnectionModel.h>
#include "ActeinService.h"

//temp
#include <gen/vr_game.pb.h>
#include "GameRunner.h"

namespace as
{
    ActeinService::ActeinService(boost_app::context & context)
        : mContext(context)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    }

    ActeinService::~ActeinService()
    {
        spdlog::drop_all();
    }

    int ActeinService::operator()()
    {
        try
        {
            //auto execPath = mContext.find<boost_app::path>()->executable_path().wstring();
            //auto args = mContext.find<boost_app::args>()->arg_vector();
            auto status = mContext.find<boost_app::status>();

            mConnectionModel = std::make_unique<actein::ConnectionModel>("test.mosquitto.org", 1);
            mConnectionModel->Start();

            mWorker = std::thread(&ActeinService::onStart, this);

            mContext.find<boost_app::wait_for_termination_request>()->wait();

            return 0;
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error(ex.what());
        }
        catch (const mqtt::persistence_exception& ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
        return 0;
    }

    void ActeinService::onStart()
    {
        try
        {
            vr_events::VrGame game;
            game.set_steam_game_id(392190);
            mTestGameRunner.Run(game);
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
    }

    bool ActeinService::stop()
    {
        try
        {
            if (mWorker.joinable())
            {
                mWorker.join();
            }
            mConnectionModel->Stop();
            if (mTestGameRunner.IsGameRunning())
            {
                mTestGameRunner.Stop();
            }
            return true;
        }
        catch (const vr_events::VrEventsException & ex)
        {
            mLogger->error(ex.what());
        }
        catch (const mqtt::persistence_exception& ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const mqtt::exception & ex)
        {
            mLogger->error("{}; Mqtt Paho error code: {}", ex.what(), ex.get_reason_code());
        }
        catch (const std::exception & ex)
        {
            mLogger->error(ex.what());
        }
        return false;
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