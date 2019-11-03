#include "pch.h"
#include "ServicesControllerBase.h"

using namespace RemoteServices;

class ServicesRemoteController final : public ServicesControllerBase
{
    using PairRequest = std::function<void(byte)>;

public:
    ServicesRemoteController() = default;
    virtual ~ServicesRemoteController() override final = default;

    virtual void CreateServices() override final;

protected:
    virtual void OnPairMessage(const ServicePayload& payload) override final;

    virtual void UnpairService(ServiceData& serviceData) override final;
    virtual void UnpairServices() override final;

private:
    void SendPairRequest(IServiceFactory& factory);
    void OnPairResponse(const std::string& serviceName, byte servicePort);

private:
    std::map<std::string, IServiceFactory&> m_pairRequests;
};

void ServicesRemoteController::CreateServices()
{
    UnpairServices();

    VisitServiceFactories([this](auto& serviceFactory)
    {
        SendPairRequest(serviceFactory);
    });
}

void ServicesRemoteController::OnPairMessage(const ServicePayload& payload)
{
    byte servicePort;
    REMOTE_SERVICES_ASSERT(payload.size() > sizeof(servicePort));
    if (payload.size() <= sizeof(servicePort))
        return;

    servicePort = *static_cast<const byte*>(payload.data());
    const auto serviceName = std::string{ payload.cbegin() + sizeof(servicePort), payload.cend() };
    OnPairResponse(serviceName, servicePort);
}

void ServicesRemoteController::UnpairService(ServiceData& serviceData)
{
    ServicesControllerBase::UnpairService(serviceData);
    serviceData.Service->Finalize();
}

void ServicesRemoteController::UnpairServices()
{
    ServicesControllerBase::UnpairServices();
    m_pairRequests.clear();
}

void ServicesRemoteController::SendPairRequest(IServiceFactory& factory)
{
    const auto& serviceName = factory.GetServiceName();
    ServicePayload payload(serviceName.cbegin(), serviceName.cend());
    const auto result = Send(s_pairServicePort, std::move(payload));
    REMOTE_SERVICES_ASSERT(result);
    if (result)
        m_pairRequests.emplace(serviceName, factory);
}

void ServicesRemoteController::OnPairResponse(const std::string& serviceName, byte servicePort)
{
    const auto iterator = m_pairRequests.find(serviceName);
    REMOTE_SERVICES_ASSERT(iterator != m_pairRequests.cend());
    if (iterator == m_pairRequests.cend())
        return;

    auto& factory = iterator->second;

    auto service = servicePort == s_pairServicePort ? nullptr : factory.Create();
    m_pairRequests.erase(iterator);

    REMOTE_SERVICES_ASSERT(servicePort == s_pairServicePort || service);
    if (!service)
        return;

    if (!service->Initialize())
    {
        REMOTE_SERVICES_ASSERT(false);
        return;
    }

    PairService(serviceName, std::move(service), servicePort);
}

IServicesControllerUniquePtr IServicesController::CreateRemote()
{
    return std::make_unique<ServicesRemoteController>();
}