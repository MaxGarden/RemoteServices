#pragma once

#include "Service.h"

namespace RemoteServices
{
    template<typename ServiceType>
    class ServiceFactory final : public IClientServiceFactory
    {
    public:
        ServiceFactory(std::string&& serviceName);
        virtual ~ServiceFactory() override final = default;

        virtual const std::string& GetServiceName() const noexcept override final;
        virtual IServiceUniquePtr Create() override final;

    private:
        const std::string m_serviceName;
    };

    #include "ServiceFactory.inl"
}