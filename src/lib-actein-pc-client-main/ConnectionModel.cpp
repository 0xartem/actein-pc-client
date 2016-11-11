#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>
#include <Connection.h>
#include <ISubscriber.h>
#include <IPublisher.h>
#include <CommonActionListener.h>
#include <gen/vr_booth_info.pb.h>
#include <MqttVrEventsManager.h>
#include <IVrEventsSubscriber.h>
#include <IVrEventsPublisher.h>
#include "ConnectionModel.h"
#include "ScheduleVrEventsHandler.h"
#include "Settings.h"

using CommonListener = mqtt_transport::CommonActionListener;
using MqttAction = mqtt_transport::Action;

namespace actein
{
    ConnectionModel::ConnectionModel(Settings & settings)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
        mConnectListener = std::make_unique<CommonListener>(MqttAction::CONNECT, this);
        mDisconnectListener = std::make_unique<CommonListener>(MqttAction::DISCONNECT, this);

        mConnection = mqtt_transport::Connection::CreateInstance(settings.GetBrokerHost());

        auto vrBoothInfo = std::make_shared<vr_events::VrBoothInfo>();
        vrBoothInfo->set_id(settings.GetBoothId());

        mVrEventsManager = std::make_unique<vr_events::MqttVrEventsManager>(*mConnection, vrBoothInfo);
        mVrEventsHandler = std::make_unique<ScheduleVrEventsHandler>(settings, *this);
    }

    ConnectionModel::~ConnectionModel()
    {
    }

    void ConnectionModel::Start()
    {
        mConnection->Connect(*mConnectListener);
    }

    void ConnectionModel::Stop()
    {
        if (mVrEventsManager->IsRunning())
        {
            mVrEventsManager->GetSubscriber()->UnsubscribeFromGameOffEvent();
            mVrEventsManager->GetSubscriber()->UnsubscribeFromGameOnEvent();
            mConnection->WaitPendingTokens();
            mVrEventsManager->Stop();
        }

        if (mConnection->GetClient().is_connected())
        {
            mConnection->Disconnect(*mDisconnectListener);
        }
    }

    vr_events::IVrEventsManager * ConnectionModel::GetVrEventsManager() const
    {
        return mVrEventsManager.get();
    }

    void ConnectionModel::OnActionSuccess(mqtt_transport::Action action, const std::string & message)
    {
        //Already logged message in the CommonActionListener
        if (action == MqttAction::CONNECT)
        {
            mVrEventsManager->Start(mVrEventsHandler.get(), this, this);
            mVrEventsManager->GetSubscriber()->SubscribeToGameOnEvent();
            mVrEventsManager->GetSubscriber()->SubscribeToGameOffEvent();
        }
    }

    void ConnectionModel::OnActionFailure(mqtt_transport::Action action, const std::string & message)
    {
        //Already logged message in the CommonActionListener
    }

    void ConnectionModel::OnConnectionLost()
    {
        mLogger->warn("MQTT connection is lost");
    }
}