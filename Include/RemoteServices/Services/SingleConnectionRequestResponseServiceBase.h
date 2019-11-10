#pragma once

#include "RequestResponseServiceBase.h"

namespace RemoteServices
{
    class SingleConnectionRequestResponseServiceBase : public RequestResponseServiceBase
    {
    public:
        SingleConnectionRequestResponseServiceBase() = default;
        virtual ~SingleConnectionRequestResponseServiceBase() override = default;

        virtual void OnBind(const IServiceConnectionSharedPtr& connection) override;
        virtual void OnUnbind(const IServiceConnectionSharedPtr& connection) override;

    protected:
        bool SendRequest(Request&& request, ResponseCallback&& responseCallback);

    private:
        IServiceConnectionSharedPtr m_connection;
    };
}