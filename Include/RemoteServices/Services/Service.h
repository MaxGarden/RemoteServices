#pragma once

#include "RemoteServices/DataModels/Services.h"

namespace RemoteServices
{
    class IServiceConnection
    {
    public:
        virtual ~IServiceConnection() = default;

        virtual bool Send(ServicePayload&& payload) = 0;
    };

    class IService
    {
    public:
        virtual ~IService() = default;

        virtual bool Initialize() = 0;

        virtual void OnBind(const IServiceConnectionSharedPtr& connection) = 0;
        virtual void OnUnbind(const IServiceConnectionSharedPtr& connection) = 0;

        virtual void Finalize() = 0;

        virtual void OnReceived(const IServiceConnectionSharedPtr& connection, const ServicePayload& payload) = 0;
    };

    class IServiceFactory
    {
    public:
        virtual ~IServiceFactory() = default;

        virtual const std::string& GetServiceName() const noexcept = 0;
        virtual IServiceUniquePtr Create() = 0;
    };
}