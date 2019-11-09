#include "pch.h"
#include "RemoteServices/Services/Service.h"
#include "RemoteServices/DataModels/Services.h"
#include "RemoteServices/DataModels/ServicesListenerBase.h"
#include "ServicesControllerBase.h"

using namespace RemoteServices;

const byte ServicesControllerBase::s_pairServicePort = 0u;

class ServicesControllerListener final : public ServicesListenerBase
{
public:
    ServicesControllerListener(ServicesControllerBase& controller);
    virtual ~ServicesControllerListener() override final = default;

    virtual void OnReceived(const Packet& packet) override final;

    virtual void OnDisconnected() override final;

private:
    ServicesControllerBase& m_controller;
};

class ServiceConnection final : public IServiceConnection
{
public:
    ServiceConnection(byte servicePort, ServicesControllerBase* controller);
    virtual ~ServiceConnection() override final = default;

    virtual bool Send(ServicePayload&& payload) override;

    void Invalidate();

private:
    const byte m_servicePort;
    ServicesControllerBase* m_controller;
};


ServicesControllerListener::ServicesControllerListener(ServicesControllerBase& controller) :
    m_controller{ controller }
{
}

void ServicesControllerListener::OnReceived(const Packet& packet)
{
    m_controller.OnReceived(packet);
}

void ServicesControllerListener::OnDisconnected()
{
    m_controller.OnDisconnected();
}

ServiceConnection::ServiceConnection(byte servicePort, ServicesControllerBase* controller) :
    m_servicePort{ servicePort },
    m_controller{ controller }
{
}

bool ServiceConnection::Send(ServicePayload&& payload)
{
    REMOTE_SERVICES_ASSERT(m_controller);
    if (!m_controller)
        return false;

    return m_controller->Send(m_servicePort, std::move(payload));
}

void ServiceConnection::Invalidate()
{
    m_controller = nullptr;
}

bool ServicesControllerBase::RegisterServiceFactory(IServiceFactoryUniquePtr&& serviceFactory)
{
    if (!serviceFactory)
        return false;

    m_servicesFactories.emplace_back(std::move(serviceFactory));
    return true;
}

void ServicesControllerBase::VisitServices(const MVC::VisitorType<IService>& visitor) const
{
    if (!visitor)
        return;

    MVC::VisitObjectsMap(m_pairedServices, [&visitor](const auto& serviceData)
    {
        if (const auto& service = serviceData.Service)
            visitor(*service);
    });
}

std::string ServicesControllerBase::GetRemoteAddress() const noexcept
{
    const auto& dataModel = GetDataModel();
    const auto remoteAddress = dataModel ? dataModel->GetRemoteAddress() : std::nullopt;
    return remoteAddress ? *remoteAddress : "No connection";
}

bool ServicesControllerBase::Disconnect()
{
    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return false;

    return dataModel->Disconnect();
}

bool ServicesControllerBase::Send(byte servicePort, ServicePayload&& payload)
{
    REMOTE_SERVICES_ASSERT(m_isConnected);
    if (!m_isConnected)
        return false;

    const auto services = GetDataModel();
    REMOTE_SERVICES_ASSERT(services);
    if (!services)
        return false;

    return services->Send(Packet{ servicePort, std::move(payload) });
}

void ServicesControllerBase::OnReceived(const Packet& packet)
{
    REMOTE_SERVICES_ASSERT(m_isConnected);
    if (!m_isConnected)
        return;

    const auto servicePort = packet.ServicePort;
    if (servicePort == s_pairServicePort)
        return OnPairMessage(packet.Payload);

    const auto iterator = m_pairedServices.find(servicePort);
    REMOTE_SERVICES_ASSERT(iterator != m_pairedServices.cend());
    if (iterator == m_pairedServices.cend())
        return;

    if (const auto& service = iterator->second.Service)
        service->OnReceived(packet.Payload);
}

void ServicesControllerBase::OnConnected()
{
    REMOTE_SERVICES_ASSERT(!m_isConnected);
    if (m_isConnected)
        return;

    m_isConnected = true;
}

void ServicesControllerBase::OnDisconnected()
{
    REMOTE_SERVICES_ASSERT(m_isConnected);
    if (!m_isConnected)
        return;

    UnpairServices();
    m_isConnected = false;
}

void ServicesControllerBase::OnDataModelChanged()
{
    const auto dataModel = GetDataModel();
    if (!dataModel)
    {
        if (m_isConnected)
            OnDisconnected();
    }
    else
    {
        if (dataModel->IsConnected() == m_isConnected)
            return;

        if (m_isConnected)
            OnDisconnected();
        else
            OnConnected();
    }
}

MVC::IListenerUniquePtr ServicesControllerBase::CreateListener()
{
    return std::make_unique<ServicesControllerListener>(*this);
}

void ServicesControllerBase::PairService(const std::string& serviceName, IServiceUniquePtr&& service, byte servicePort)
{
    const auto serviceRaw = service.get();
    const auto serviceConnection = std::make_shared<ServiceConnection>(servicePort, this);
    m_pairedServices.emplace(servicePort, ServiceData{ servicePort, serviceName, std::move(service), serviceConnection });

    serviceRaw->OnBind(serviceConnection);
    NotifyListeners(&IServicesControllerListener::OnServicePaired, *serviceRaw);
}

void ServicesControllerBase::UnpairService(ServiceData& serviceData)
{
    auto& service = serviceData.Service;
    if (!service)
        return;

    auto& connection = serviceData.Connection;

    service->OnUnbind(connection);

    if (const auto serviceConnection = dynamic_cast<ServiceConnection*>(connection.get()))
        serviceConnection->Invalidate();

    connection.reset();

    NotifyListeners(&IServicesControllerListener::OnServiceUnparied, *service);
    OnServiceUnpaired(serviceData);

    m_pairedServices.erase(serviceData.ServicePort);
}

void ServicesControllerBase::UnpairServices()
{
    std::vector<byte> pairedServicesPorts;
    pairedServicesPorts.reserve(m_pairedServices.size());

    for (const auto& pair : m_pairedServices)
        pairedServicesPorts.emplace_back(pair.first);

    for (const auto& servicePort : pairedServicesPorts)
        UnpairService(m_pairedServices[servicePort]);

    m_pairedServices.clear(); //just to be sure
}

void ServicesControllerBase::VisitServiceFactories(const MVC::VisitorType<IServiceFactory&>& visitor)
{
    MVC::VisitPointersContainer(m_servicesFactories, visitor);
}

std::optional<byte> ServicesControllerBase::GetFreeServicePort() const noexcept
{
    constexpr auto max = std::numeric_limits<byte>::max();

    for (auto i = 1u; i < max; ++i)
    {
        if (m_pairedServices.find(i) == m_pairedServices.cend())
            return i;
    }

    return std::nullopt;
}

void ServicesControllerBase::OnServiceUnpaired(ServiceData& serviceData)
{
    //to override
}