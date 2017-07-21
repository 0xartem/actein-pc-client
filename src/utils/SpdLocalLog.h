#ifndef SPD_LOCAL_LOG_H__
#define SPD_LOCAL_LOG_H__

#include <spdlog/spdlog.h>

static const char * COMMON_LOGGER_NAME = "common_logger";

#define LOG_ERROR(logger, errorText) \
{ \
    logger->error("FUNC: {}; LINE: {}; ERR: {}", __FUNCTION__, __LINE__, errorText); \
}

#define LOG_ERROR_WITH_ERROR_CODE(logger, errorStr, errorCodePrefix, errorCode) \
{ \
    mLogger->error( \
        "FUNC: {}; LINE: {}; ERR: {}; {}: {}", \
        __FUNCTION__, __LINE__, \
        errorStr, errorCodePrefix, errorCode \
    ); \
}

#endif //SPD_LOCAL_LOG_H__