#ifndef EMB_DEVICE_LAST_WILL_SUBSCRIBER_H__
#define EMB_DEVICE_LAST_WILL_SUBSCRIBER_H__

#include "IMessageHandler.h"

namespace mqtt_transport
{
    class Connection;
    class IActionStatusObserver;
    class IEmbDeviceOnlineStatusHandler;
    class CommonActionListener;

    class LastWillManager : public IMessageHandler
    {
    public:
        LastWillManager(
            mqtt_transport::Connection & connection,
            IActionStatusObserver * actionObserver,
            IEmbDeviceOnlineStatusHandler * embDeviceStatusHandler,
            int boothId
        );

        void Start();
        void Stop();
        bool IsRunning() const;

        // throws mqtt::exception
        void PublishPcOnlineStatus(bool online);
        // throws mqtt::exception
        void SubscribeToEmbDeviceLastWill();
        // throws mqtt::exception
        void UnsubscribeFromEmbDeviceLastWill();
        void HandleMessage(const std::string & topic, mqtt::message_ptr message) override;

    private:
        void ProcessOnlineStatusEvent(mqtt::message_ptr message);

    private:
        bool mIsRunning;
        mutable std::mutex mSync;

        std::string mPcLastWillTopic;
        std::string mEmbDeviceLastWillTopic;

        IPublisher & mPublisher;
        ISubscriber & mSubscriber;
        IEmbDeviceOnlineStatusHandler * mEmbDeviceStatusHandler;

        std::unique_ptr<CommonActionListener> mPublishListener;
        std::unique_ptr<CommonActionListener> mSubscribeListener;
        std::unique_ptr<CommonActionListener> mUnsubscribeListener;

        std::shared_ptr<spdlog::logger> mLogger;
    };
}

#endif //EMB_DEVICE_LAST_WILL_SUBSCRIBER_H__
