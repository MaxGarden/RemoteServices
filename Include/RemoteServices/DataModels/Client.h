#pragma once

#include "MVC/DataModel.h"
#include "RemoteServices/DataModels/Services.h"
#include "Networking/Client/Client.h"

namespace RemoteServices
{
    class IClientListener : public MVC::IListener
    {
    public:
        virtual ~IClientListener() override = default;

        virtual void OnServerConnected(const RemoteServices::IServicesSharedPtr& services) = 0;
        virtual void OnServerDisconnected(const RemoteServices::IServicesSharedPtr& services) = 0;
    };

    class IClient : public MVC::IDataModel
    {
    public:
        virtual ~IClient() override = default;

        virtual bool Initialize() = 0;
        virtual void Finalize() = 0;

        virtual bool ConnectToServer(const std::string& address, unsigned short port, size_t timeoutInMiliseconds = 500) = 0;

        virtual void VisitServers(const MVC::ConstVisitorType<RemoteServices::IServicesSharedPtr>& visitor) = 0;

        virtual void Update() = 0;

        static IClientUniquePtr Create(Networking::IClientUniquePtr&& client);
    };
}