#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>
#include "ConnectionModel.h"
#include <Connection.h>
#include <ISubscriber.h>
#include <IPublisher.h>
#include <CommonActionListener.h>
#include <gen/vr_booth_info.pb.h>
#include <gen/vr_game_on_event.pb.h>
#include <gen/vr_game_status_event.pb.h>
#include <MqttVrEventsManager.h>
#include <IVrEventsSubscriber.h>
#include <IVrEventsPublisher.h>

#include "GameRunner.h"

using CommonListener = mqtt_transport::CommonActionListener;
using MqttAction = mqtt_transport::Action;

ConnectionModel::ConnectionModel(const std::string & brokerHost, int boothId)
{
    mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
    mConnectListener = std::make_unique<CommonListener>(MqttAction::CONNECT, this);
    mDisconnectListener = std::make_unique<CommonListener>(MqttAction::DISCONNECT, this);

    mConnection = mqtt_transport::Connection::CreateInstance(brokerHost);

    auto vrBoothInfo = std::make_shared<vr_events::VrBoothInfo>();
    vrBoothInfo->set_id(boothId);

    mVrEventsManager = std::make_unique<vr_events::MqttVrEventsManager>(*mConnection, vrBoothInfo);
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
        mVrEventsManager->Start(this, this, this);
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

void ConnectionModel::HandleVrGameOnEvent(const std::shared_ptr<vr_events::VrGameOnEvent> & event)
{
    mLogger->info("VR game on event received. Game {}", event->game().game_name());

    GameRunner gameRunner(event->game());
    gameRunner.Run();

    mVrEventsManager->GetPublisher()->PublishVrGameStatusEvent(vr_events::VrGameStatus::GAME_ON);
}

void ConnectionModel::HandleVrGameOffEvent(const std::shared_ptr<vr_events::VrGameOffEvent> & event)
{
    mLogger->info("VR game off event received.");
    mVrEventsManager->GetPublisher()->PublishVrGameStatusEvent(vr_events::VrGameStatus::GAME_OFF);
}

void ConnectionModel::HandleVrGameStatusEvent(const std::shared_ptr<vr_events::VrGameStatusEvent> & event)
{
    mLogger->info("VR game status event received");
}
