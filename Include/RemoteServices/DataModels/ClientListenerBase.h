#pragma once

#include "Client.h"

namespace RemoteServices
{
    class ClientListenerBase : public IClientListener
    {
    public:
        ClientListenerBase() = default;
        virtual ~ClientListenerBase() override = default;

        virtual void OnServerConnected(const RemoteServices::IServicesSharedPtr& services) override;
        virtual void OnServerDisconnected(const RemoteServices::IServicesSharedPtr& services) override;
    };
}