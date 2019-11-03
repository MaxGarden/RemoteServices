#pragma once

#include "Server.h"

namespace RemoteServices
{
    class ServerListenerBase : public IServerListener
    {
    public:
        ServerListenerBase() = default;
        virtual ~ServerListenerBase() override = default;

        virtual void OnClientConnected(const RemoteServices::IServicesSharedPtr& services) override;
        virtual void OnClientDisconnected(const RemoteServices::IServicesSharedPtr& services) override;
    };
}