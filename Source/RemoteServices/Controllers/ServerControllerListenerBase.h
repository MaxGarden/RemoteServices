#pragma once

#include "ServerController.h"

namespace RemoteServices
{
    class ServerControllerListenerBase : public IServerControllerListener
    {
    public:
        ServerControllerListenerBase() = default;
        virtual ~ServerControllerListenerBase() override = default;

        virtual void OnClientConnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) override;
        virtual void OnClientDisconnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) override;
    };
}