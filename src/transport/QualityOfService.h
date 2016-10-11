#ifndef QUALITY_OF_SERVICE_H__
#define QUALITY_OF_SERVICE_H__

namespace transport
{
    enum class QualityOfService
    {
        AT_MOST_ONCE_DELIVERY_GUARANTEE,
        AT_LEAST_ONCE_DELIVERY_GUARANTEE,
        EXACTLY_ONCE_DELIVERY_GUARANTEE
    };

    static int convertToMqttQosValues(QualityOfService qualityOfService)
    {
        switch (qualityOfService)
        {
        case QualityOfService::AT_MOST_ONCE_DELIVERY_GUARANTEE:
            return 0;
        case QualityOfService::AT_LEAST_ONCE_DELIVERY_GUARANTEE:
            return 1;
        case QualityOfService::EXACTLY_ONCE_DELIVERY_GUARANTEE:
            return 2;
        default:
            throw std::logic_error("Unknown quality of service type");
        }
    }
}

#endif //QUALITY_OF_SERVICE_H__
