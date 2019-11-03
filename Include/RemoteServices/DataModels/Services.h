#pragma once

#include "RemoteServices/RemoteServices.h"
#include "Networking/Connection.h"
#include "MVC/DataModel.h"

namespace RemoteServices
{
    using ServicePayload = std::vector<byte>;
    struct Packet
    {
        byte ServicePort;
        ServicePayload Payload;
    };

    class IServicesListener : public MVC::IListener
    {
    public:
        virtual ~IServicesListener() override = default;

        virtual void OnReceived(const Packet& packet) = 0;
        virtual void OnDisconnected() = 0;
    };

    class IServices : public MVC::IDataModel
    {
    public:
        virtual ~IServices() override = default;

        virtual bool IsConnected() const noexcept = 0;
        virtual bool Send(Packet&& packet) = 0;

        virtual bool Disconnect() = 0;

        static IServicesUniquePtr Create(const Networking::IConnectionSharedPtr& connection);
    };
}