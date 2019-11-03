#pragma once

#include "MVC/Controller.h"
#include "RemoteServices/Controllers/ServicesController.h"

namespace RemoteServices
{
    class IServerControllerListener : public MVC::IListener
    {
    public:
        virtual ~IServerControllerListener() override = default;

        virtual void OnClientConnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) = 0;
        virtual void OnClientDisconnected(const RemoteServices::IServicesControllerSharedPtr& servicesController) = 0;
    };

    class IServerController : public MVC::IController
    {
    public:
        virtual ~IServerController() override = default;

        virtual void VisitClients(const MVC::ConstVisitorType<RemoteServices::IServicesControllerSharedPtr>& visitor) = 0;

        static IServerControllerUniquePtr Create();
    };
}