#pragma once

#include "MVC/DataModel.h"
#include "RemoteServices/DataModels/Services.h"
#include "Networking/Server/Server.h"

namespace RemoteServices
{
    class IServerListener : public MVC::IListener
    {
    public:
        virtual ~IServerListener() override = default;

        virtual void OnClientConnected(const RemoteServices::IServicesSharedPtr& services) = 0;
        virtual void OnClientDisconnected(const RemoteServices::IServicesSharedPtr& services) = 0;
    };

    class IServer : public MVC::IDataModel
    {
    public:
        virtual ~IServer() override = default;

        virtual bool Initialize() = 0;
        virtual void Finalize() = 0;

        virtual void VisitClients(const MVC::ConstVisitorType<RemoteServices::IServicesSharedPtr>& visitor) = 0;

        virtual void Update() = 0;

        static IServerUniquePtr Create(Networking::IServerUniquePtr&& server);
    };
}