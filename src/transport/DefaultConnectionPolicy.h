#ifndef DEFAULT_CONNECTION_POLICY_H__
#define DEFAULT_CONNECTION_POLICY_H__

#include "IConnectionPolicy.h"

namespace transport
{
    class DefaultConnectionPolicy : public IConnectionPolicy
    {
    public:
        virtual bool isPersistentSession() const override
        {
            return PERSISTENT_SESSION_DEFAULT;
        }

        virtual int getKeepAliveInterval() const override
        {
            return KEEP_ALIVE_INTERVAL_DEFAULT;
        }

        virtual int getConnectionTimeout() const override
        {
            return CONNECTION_TIMEOUT_DEFAULT;
        }

    private:
        static const bool PERSISTENT_SESSION_DEFAULT = false;
        static const int CONNECTION_TIMEOUT_DEFAULT = 30;
        static const int KEEP_ALIVE_INTERVAL_DEFAULT = 60;
    };
}

#endif //DEFAULT_CONNECTION_POLICY_H__
