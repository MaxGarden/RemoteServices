#pragma once

#include "ServicesController.h"

namespace RemoteServices
{
    class ServicesControllerListenerBase : public IServicesControllerListener
    {
    public:
        virtual ~ServicesControllerListenerBase() override = default;

        virtual void OnServicePaired(IService& service) override;
        virtual void OnServiceUnparied(IService& service) override;
    };
}