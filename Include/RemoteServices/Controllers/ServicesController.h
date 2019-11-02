#pragma once

#include "MVC/Controller.h"
#include "Service.h"

namespace RemoteServices
{
    class IServicesControllerListener : public MVC::IListener
    {
    public:
        virtual ~IServicesControllerListener() override = default;

        virtual void OnServicePaired(IService& service) = 0;
        virtual void OnServiceUnparied(IService& service) = 0;
    };

    class IServicesController : public MVC::IController
    {
    public:
        virtual ~IServicesController() override = default;

        virtual bool RegisterServiceFactory(IServiceFactoryUniquePtr&& serviceFactory) = 0;
        virtual void VisitServices(const MVC::ConstVisitorType<IService>& visitor) const = 0;

        virtual void CreateServices() = 0;

        virtual bool Disconnect() = 0;

        static IServicesControllerUniquePtr CreateLocal(); //all registered services are created - server mode
        static IServicesControllerUniquePtr CreateRemote(); //only services which are running on server are created and paired - client mode
    };
}