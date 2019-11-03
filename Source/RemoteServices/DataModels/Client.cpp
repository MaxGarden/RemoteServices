#include "pch.h"
#include "RemoteServices/DataModels/Client.h"
#include "MVC/ListenableBase.h"

using namespace RemoteServices;

class Client final : public MVC::ListenableBase<IClient, IClientListener>
{
public:
    Client() = default;
    virtual ~Client() override final;

    bool SetClient(Networking::IClientUniquePtr&& client);

    virtual bool Initialize() override final;
    virtual void Finalize() override final;

    virtual bool ConnectToServer(const std::string& address, unsigned short port, size_t timeoutInMiliseconds) override final;

    virtual void VisitServers(const MVC::ConstVisitorType<RemoteServices::IServicesSharedPtr>& visitor) override final;

    virtual void Update() override final;

private:
    void OnServerDisconnected(const Networking::IConnectionSharedPtr& connection);

private:
    Networking::IClientUniquePtr m_client;
    std::map<Networking::IConnectionSharedPtr, RemoteServices::IServicesSharedPtr> m_servers;
};

Client::~Client()
{
    if (m_client)
        m_client->Finalize();
}

bool Client::SetClient(Networking::IClientUniquePtr&& client)
{
    if (!client)
        return false;

    m_client = std::move(client);

    m_client->SetOnConnectionClosedCallback([this](const auto& connection)
    {
        OnServerDisconnected(connection);
    });

    return true;
}

bool Client::Initialize()
{
    return m_client->Initialize();
}

void Client::Finalize()
{
    m_client->Finalize();
}

bool Client::ConnectToServer(const std::string& address, unsigned short port, size_t timeoutInMiliseconds)
{
    const auto connection = m_client->Connect(address, port, timeoutInMiliseconds);
    if (!connection || !connection->IsConnected())
        return false;

    auto services = RemoteServices::IServices::Create(connection);
    REMOTE_SERVICES_ASSERT(services);

    m_servers.emplace(connection, std::move(services));
    NotifyListeners(&IClientListener::OnServerConnected, m_servers[connection]);
    return true;
}

void Client::VisitServers(const MVC::ConstVisitorType<RemoteServices::IServicesSharedPtr>& visitor)
{
    VisitObjectsMap(m_servers, visitor);
}

void Client::Update()
{
    m_client->Update();
}

void Client::OnServerDisconnected(const Networking::IConnectionSharedPtr& connection)
{
    const auto iterator = m_servers.find(connection);
    REMOTE_SERVICES_ASSERT(iterator != m_servers.cend());
    if (iterator == m_servers.cend())
        return;

    NotifyListeners(&IClientListener::OnServerDisconnected, iterator->second);
    m_servers.erase(iterator);
}

IClientUniquePtr IClient::Create(Networking::IClientUniquePtr&& client)
{
    auto result = std::make_unique<Client>();
    if (!result->SetClient(std::move(client)))
        return nullptr;

    return result;
}