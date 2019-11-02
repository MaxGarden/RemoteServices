#pragma once

#include "Services.h"

namespace RemoteServices
{
    class ServicesListenerBase : public IServicesListener
    {
    public:
        virtual ~ServicesListenerBase() override = default;

        virtual void OnReceived(const Packet& packet) override;

        virtual void OnDisconnected() override;
    };
}