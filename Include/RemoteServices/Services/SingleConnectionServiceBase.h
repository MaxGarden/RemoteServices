#pragma once

#include "Service.h"

namespace RemoteServices
{
    class SingleConnectionServiceBase : public IService
    {
    public:
        SingleConnectionServiceBase() = default;
        virtual ~SingleConnectionServiceBase() override = default;

        virtual bool Initialize() override;

        virtual void OnBind(const IServiceConnectionSharedPtr& connection) override;
        virtual void OnUnbind(const IServiceConnectionSharedPtr& connection) override;

        virtual void Finalize() override;

        virtual void OnReceived(const IServiceConnectionSharedPtr& connection, const ServicePayload& payload) override final;

    protected:
        virtual void OnReceived(const ServicePayload& payload) = 0;

        bool Send(ServicePayload&& payload);
        const IServiceConnectionSharedPtr& GetConnection() const noexcept;

    private:
        IServiceConnectionSharedPtr m_Connection;
    };
}