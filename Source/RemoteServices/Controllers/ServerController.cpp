#include "pch.h"
#include "RemoteServices/Controllers/ServerController.h"
#include "RemoteServices/DataModels/ServerListenerBase.h"
#include "MVC/ControllerBase.h"
#include "RemoteServices/Controllers/ServicesController.h"

using namespace RemoteServices;

class ServerController;
class ServerControllerListener final : public ServerListenerBase
{
public:
    ServerControllerListener(ServerController& serverController);
    virtual ~ServerControllerListener() override = default;

    virtual void OnClientConnected(const RemoteServices::IServicesSharedPtr& client) override final;
    virtual void OnClientDisconnected(const RemoteServices::IServicesSharedPtr& client) override final;

private:
    ServerController& m_serverController;
};

class ServerController final : public MVC::ControllerBase<IServerController, IServer, IServerControllerListener>
{
public:
    ServerController() = default;
    virtual ~ServerController() override = default;

    virtual void VisitClients(const MVC::ConstVisitorType<RemoteServices::IServicesControllerSharedPtr>& visitor) override final;

    void OnClientConnected(const RemoteServices::IServicesSharedPtr& client);
    void OnClientDisconnected(const RemoteServices::IServicesSharedPtr& client);

protected:
    virtual void OnDataModelChanged() override final;
    virtual MVC::IListenerUniquePtr CreateListener() override final;

private:
    std::map<RemoteServices::IServicesSharedPtr, RemoteServices::IServicesControllerSharedPtr> m_clientsControllers;
};

ServerControllerListener::ServerControllerListener(ServerController& serverController) :
    m_serverController(serverController)
{
}

void ServerControllerListener::OnClientConnected(const RemoteServices::IServicesSharedPtr& client)
{
    m_serverController.OnClientConnected(client);
}

void ServerControllerListener::OnClientDisconnected(const RemoteServices::IServicesSharedPtr& client)
{
    m_serverController.OnClientDisconnected(client);
}

void ServerController::VisitClients(const MVC::ConstVisitorType<RemoteServices::IServicesControllerSharedPtr>& visitor)
{
    VisitObjectsMap(m_clientsControllers, visitor);
}

void ServerController::OnClientConnected(const RemoteServices::IServicesSharedPtr& client)
{
    const auto clientController = RemoteServices::IServicesControllerSharedPtr{ RemoteServices::IServicesController::CreateLocal() };
    REMOTE_SERVICES_ASSERT(clientController);
    if (!clientController)
        return;

    if (!clientController->SetDataModel(client))
    {
        REMOTE_SERVICES_ASSERT(false);
        return;
    }

    m_clientsControllers.emplace(client, clientController);
    NotifyListeners(&IServerControllerListener::OnClientConnected, clientController);
}

void ServerController::OnClientDisconnected(const RemoteServices::IServicesSharedPtr& client)
{
    const auto iterator = m_clientsControllers.find(client);
    REMOTE_SERVICES_ASSERT(iterator != m_clientsControllers.cend());
    if (iterator == m_clientsControllers.cend())
        return;

    const auto& clientController = iterator->second;
    clientController->SetDataModel(nullptr);

    NotifyListeners(&IServerControllerListener::OnClientDisconnected, clientController);
    m_clientsControllers.erase(iterator);
}

void ServerController::OnDataModelChanged()
{
    while (!m_clientsControllers.empty())
    {
        const auto iterator = m_clientsControllers.begin();
        if (!iterator->first || !iterator->second)
            m_clientsControllers.erase(iterator);
        else
            OnClientDisconnected(iterator->first);
    }

    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return;

    dataModel->VisitClients([this](const auto& client)
    {
        OnClientConnected(client);
    });
}

MVC::IListenerUniquePtr ServerController::CreateListener()
{
    return std::make_unique<ServerControllerListener>(*this);
}

IServerControllerUniquePtr IServerController::Create()
{
    return std::make_unique<ServerController>();
}