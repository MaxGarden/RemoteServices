#pragma once

#include "Controllers/ServicesController.h"

namespace RemoteServices
{
    class IServicesBuilder
    {
    public:
        virtual ~IServicesBuilder() = default;

        virtual bool Build(IServicesController& controller) = 0;
    };

    class IServicesBuildersProvider
    {
    public:
        virtual ~IServicesBuildersProvider() = default;

        virtual bool RegisterBuilder(byte deviceId, IServicesBuilderUniquePtr&& builder) = 0;
        virtual bool UnregisterBuilder(byte deviceId) = 0;

        virtual IServicesBuilder* GetBuilder(byte deviceId) const noexcept = 0;

        static IServicesBuildersProviderUniquePtr Create();
    };
}