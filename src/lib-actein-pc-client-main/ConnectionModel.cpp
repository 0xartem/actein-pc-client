#include <spdlog/spdlog.h>
#include <mqtt/async_client.h>
#include <Connection.h>
#include <ISubscriber.h>
#include <IPublisher.h>
#include <MqttSubscriberCallback.h>
#include <CommonActionListener.h>
#include <TopicBuilder.h>
#include <PreciseDeliveryConnectionPolicy.h>
#include <LastWillManager.h>
#include <gen/vr_booth_info.pb.h>
#include <MqttVrEventsManager.h>
#include <IVrEventsSubscriber.h>
#include <IVrEventsPublisher.h>
#include "ConnectionModel.h"
#include "ScheduleVrEventsHandler.h"
#include "Settings.h"

using CommonListener = mqtt_transport::CommonActionListener;
using MqttCallback = mqtt_transport::MqttSubscriberCallback;
using PreciseDeliveryPolicy = mqtt_transport::PreciseDeliveryConnectionPolicy;
using LwManager = mqtt_transport::LastWillManager;

namespace actein
{
    ConnectionModel::ConnectionModel(Settings & settings)
        : mSettings(settings)
    {
        mLogger = spdlog::get(spdlog::COMMON_LOGGER_NAME);
        mConnectListener = std::make_unique<CommonListener>(MqttAction::CONNECT, this);
        mDisconnectListener = std::make_unique<CommonListener>(MqttAction::DISCONNECT, this);

        mConnection = mqtt_transport::Connection::CreateInstance(
            settings.GetBrokerHost(),
            std::make_unique<PreciseDeliveryPolicy>(settings.GetBoothId())
        );

        mLastWillManager = std::make_unique<LwManager>(
            *mConnection,
            this, this,
            settings.GetBoothId()
            );

        mVrEventsHandler = std::make_unique<ScheduleVrEventsHandler>(settings, *this);

        auto vrBoothInfo = std::make_shared<vr_events::VrBoothInfo>();
        vrBoothInfo->set_id(settings.GetBoothId());

        mVrEventsManager = std::make_unique<vr_events::MqttVrEventsManager>(
            *mConnection,
            mVrEventsHandler.get(),
            this,
            vrBoothInfo
            );

        mMessageHandlers.insert(std::make_pair("last-will", mLastWillManager.get()));
        mMessageHandlers.insert(std::make_pair("vr-events", mVrEventsManager->GetMessageHandler()));
    }

    ConnectionModel::~ConnectionModel()
    {
        mMessageHandlers.clear();
    }

    void ConnectionModel::Start()
    {
        std::unique_lock<std::mutex> locker(mSync);
        std::unique_ptr<MqttCallback> callback = std::make_unique<MqttCallback>(this, this);
        mConnection->GetSubcriber().SetupCallback(std::move(callback));
        mConnection->Connect(*mConnectListener);
    }

    void ConnectionModel::Stop()
    {
        std::unique_lock<std::mutex> locker(mSync);
        mLastWillManager->Stop();
        if (mVrEventsManager->IsRunning())
        {
            mVrEventsManager->Stop();
            mConnection->WaitPendingTokens();
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

    void ConnectionModel::OnActionSuccess(MqttAction action, const std::string & message)
    {
        //Already logged message in the CommonActionListener
        if (action == MqttAction::CONNECT)
        {
            mLastWillManager->Start();
            mVrEventsManager->Start();
            mVrEventsHandler->OnStart();
        }
    }

    void ConnectionModel::OnActionFailure(MqttAction action, const std::string & message)
    {
        //Already logged message in the CommonActionListener
    }

    void ConnectionModel::OnConnectionLost()
    {
        mLogger->warn("MQTT connection is lost");
    }

    void ConnectionModel::HandleMessage(const std::string & topic, mqtt::message_ptr message)
    {
        for (const auto & handler : mMessageHandlers)
        {
            handler.second->HandleMessage(topic, message);
        }
    }

    void ConnectionModel::OnEmbDeviceOnlineStatusChanged(mqtt_transport::OnlineStatus status)
    {
        mLogger->info("Embedded device online status changed: {}", status);
    }
}