#pragma once

#include "MVC/Controller.h"
#include "RemoteServices/Controllers/ServicesController.h"

namespace RemoteServices
{
    class IClientControllerListener : public MVC::IListener
    {
    public:
        virtual ~IClientControllerListener() override = default;

        virtual void OnServerConnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) = 0;
        virtual void OnServerDisconnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) = 0;
    };

    class IClientController : public MVC::IController
    {
    public:
        virtual ~IClientController() override = default;

        virtual void VisitServers(const MVC::ConstVisitorType<RemoteServices::IServicesControllerSharedPtr>& visitor) = 0;

        static IClientControllerUniquePtr Create();
    };
}