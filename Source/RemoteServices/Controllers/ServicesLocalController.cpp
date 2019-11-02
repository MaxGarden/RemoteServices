#include "pch.h"
#include "ServicesControllerBase.h"

using namespace RemoteServices;

class ServicesLocalController final : public ServicesControllerBase
{
    using PairRequest = std::function<void(byte)>;

public:
    ServicesLocalController() = default;
    virtual ~ServicesLocalController() override final;

    virtual void CreateServices() override final;

protected:
    virtual void OnPairMessage(const ServicePayload& payload) override final;
    virtual void UnpairService(ServiceData& serviceData) override final;

private:
    void OnPairRequest(const std::string& serviceName);

    void CreateService(IServiceFactory& factory);
    void DestroyServices();

private:
    std::map<std::string, IServiceUniquePtr> m_createdServices;
};

ServicesLocalController::~ServicesLocalController()
{
    DestroyServices();
}

void ServicesLocalController::CreateServices()
{
    DestroyServices();

    VisitServiceFactories([this](auto& serviceFactory)
    {
        CreateService(serviceFactory);
    });
}

void ServicesLocalController::CreateService(IServiceFactory& factory)
{
    auto service = factory.Create();
    REMOTE_SERVICES_ASSERT(service);
    if (!service)
        return;

    if (!service->Initialize())
    {
        REMOTE_SERVICES_ASSERT(false);
        return;
    }

    m_createdServices.emplace(factory.GetServiceName(), std::move(service));
}

void ServicesLocalController::OnPairMessage(const ServicePayload& payload)
{
    REMOTE_SERVICES_ASSERT(!payload.empty());
    if (payload.empty())
        return;

    std::string serviceName;
    serviceName.resize(payload.size());

    memcpy(reinterpret_cast<void*>(serviceName.data()), payload.data(), serviceName.size());
    OnPairRequest(serviceName);
}

void ServicesLocalController::UnpairService(ServiceData& serviceData)
{
    ServicesControllerBase::UnpairService(serviceData);
    m_createdServices.emplace(serviceData.ServiceName, std::move(serviceData.Service));
}

void ServicesLocalController::OnPairRequest(const std::string& serviceName)
{
    const auto freeServicePort = GetFreeServicePort();
    const auto iterator = m_createdServices.find(serviceName);
    if (!freeServicePort || iterator == m_createdServices.end())
    {
        ServicePayload payload(serviceName.cbegin(), serviceName.cend());
        payload.insert(payload.begin(), s_pairServicePort);
        Send(s_pairServicePort, std::move(payload));
    }
    else
    {
        PairService(serviceName, std::move(iterator->second), *freeServicePort);
        m_createdServices.erase(iterator);
    }
}

void ServicesLocalController::DestroyServices()
{
    UnpairServices();
    
    MVC::VisitPointersMap(m_createdServices, [](auto& service)
    {
        service.Finalize();
    });

    m_createdServices.clear();
}

IServicesControllerUniquePtr IServicesController::CreateLocal()
{
    return std::make_unique<ServicesLocalController>();
}