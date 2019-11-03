#pragma once

#include "RemoteServices/Controllers/ClientController.h"

namespace RemoteServices
{
    class ClientControllerListenerBase : public IClientControllerListener
    {
    public:
        ClientControllerListenerBase() = default;
        virtual ~ClientControllerListenerBase() override = default;

        virtual void OnServerConnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) override;
        virtual void OnServerDisconnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) override;
    };
}