#include "pch.h"
#include "Server.h"
#include "MVC/ListenableBase.h"
#include "RemoteServices/DataModels/ServicesListenerBase.h"

using namespace RemoteServices;

class Server final : public MVC::ListenableBase<IServer, IServerListener>
{
public:
    Server() = default;
    virtual ~Server() override final;

    bool SetServer(Networking::IServerUniquePtr&& server);

    virtual bool Initalize() override final;
    virtual void Finalize() override final;

    virtual void VisitClients(const MVC::ConstVisitorType<RemoteServices::IServicesSharedPtr>& visitor) override final;

private:
    void OnClientConnected(const Networking::IConnectionSharedPtr& connection);
    void OnClientDisconnected(const Networking::IConnectionSharedPtr& connection);

private:
    Networking::IServerUniquePtr  m_server;
    std::map<Networking::IConnectionSharedPtr, RemoteServices::IServicesSharedPtr> m_clients;
};

Server::~Server()
{
    if(m_server)
        m_server->Finalize();
}

bool Server::SetServer(Networking::IServerUniquePtr&& server)
{
    if (!server)
        return false;

    m_server = std::move(server);
    
    m_server->SetOnClientConnectedCallback([this](const auto& connection)
    {
        OnClientConnected(connection);
    });

    m_server->SetOnConnectionClosedCallback([this](const auto& connection)
    {
        OnClientDisconnected(connection);
    });

    return true;
}

bool Server::Initalize()
{
    return m_server->Initialize();
}

void Server::Finalize()
{
    return m_server->Finalize();
}

void Server::VisitClients(const MVC::ConstVisitorType<RemoteServices::IServicesSharedPtr>& visitor)
{
    VisitObjectsMap(m_clients, visitor);
}

void Server::OnClientConnected(const Networking::IConnectionSharedPtr& connection)
{
    auto services = RemoteServices::IServices::Create(connection);
    REMOTE_SERVICES_ASSERT(services);

    m_clients.emplace(connection, std::move(services));
    NotifyListeners(&IServerListener::OnClientConnected, m_clients[connection]);
}

void Server::OnClientDisconnected(const Networking::IConnectionSharedPtr& connection)
{
    const auto iterator = m_clients.find(connection);
    REMOTE_SERVICES_ASSERT(iterator != m_clients.cend());
    if (iterator == m_clients.cend())
        return;

    NotifyListeners(&IServerListener::OnClientDisconnected, iterator->second);
    m_clients.erase(iterator);
}

IServerUniquePtr IServer::Create(Networking::IServerUniquePtr&& server)
{
    auto result = std::make_unique<Server>();
    if (!result->SetServer(std::move(server)))
        return nullptr;

    return result;
}