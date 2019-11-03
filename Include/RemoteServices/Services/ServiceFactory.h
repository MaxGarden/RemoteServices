#pragma once

#include "Service.h"

namespace RemoteServices
{
    template<typename ServiceType>
    class ClientServiceFactory final : public IClientServiceFactory
    {
    public:
        ClientServiceFactory(std::string&& serviceName);
        virtual ~ClientServiceFactory() override final = default;

        virtual const std::string& GetServiceName() const noexcept override final;
        virtual IServiceUniquePtr Create() override final;

    private:
        const std::string m_serviceName;
    };

    #include "ServiceFactory.inl"
}