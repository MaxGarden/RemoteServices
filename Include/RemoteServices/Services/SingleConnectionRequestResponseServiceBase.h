#pragma once

#include "RequestResponseServiceBase.h"

namespace RemoteServices
{
    class SingleConnectionRequestResponseServiceBase : public RequestResponseServiceBase
    {
    public:
        SingleConnectionRequestResponseServiceBase() = default;
        virtual ~SingleConnectionRequestResponseServiceBase() override = default;

        virtual bool Initialize() override;

        virtual void OnBind(const IServiceConnectionSharedPtr& connection) override;
        virtual void OnUnbind(const IServiceConnectionSharedPtr& connection) override;

        virtual void Finalize() override;

    protected:
        bool SendRequest(Request&& request, ResponseCallback&& responseCallback);

    private:
        IServiceConnectionSharedPtr m_connection;
    };
}