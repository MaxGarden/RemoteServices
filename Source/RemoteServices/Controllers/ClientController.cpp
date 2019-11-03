#include "pch.h"
#include "RemoteServices/Controllers/ClientController.h"
#include "RemoteServices/DataModels/ClientListenerBase.h"
#include "MVC/ControllerBase.h"
#include "RemoteServices/Controllers/ServicesController.h"

using namespace RemoteServices;

class ClientController;
class ClientControllerListener final : public ClientListenerBase
{
public:
    ClientControllerListener(ClientController& clientController);
    virtual ~ClientControllerListener() override = default;

    virtual void OnServerConnected(const RemoteServices::IServicesSharedPtr& client) override final;
    virtual void OnServerDisconnected(const RemoteServices::IServicesSharedPtr& client) override final;

private:
    ClientController& m_clientController;
};

class ClientController final : public MVC::ControllerBase<IClientController, IClient, IClientControllerListener>
{
public:
    ClientController() = default;
    virtual ~ClientController() override = default;

    virtual void VisitServers(const MVC::ConstVisitorType<RemoteServices::IServicesControllerSharedPtr>& visitor) override final;

    void OnServerConnected(const RemoteServices::IServicesSharedPtr& client);
    void OnServerDisconnected(const RemoteServices::IServicesSharedPtr& client);

protected:
    virtual void OnDataModelChanged() override final;
    virtual MVC::IListenerUniquePtr CreateListener() override final;

private:
    std::map<RemoteServices::IServicesSharedPtr, RemoteServices::IServicesControllerSharedPtr> m_serversControllers;
};

ClientControllerListener::ClientControllerListener(ClientController& clientController) :
    m_clientController(clientController)
{
}

void ClientControllerListener::OnServerConnected(const RemoteServices::IServicesSharedPtr& client)
{
    m_clientController.OnServerConnected(client);
}

void ClientControllerListener::OnServerDisconnected(const RemoteServices::IServicesSharedPtr& client)
{
    m_clientController.OnServerDisconnected(client);
}

void ClientController::VisitServers(const MVC::ConstVisitorType<RemoteServices::IServicesControllerSharedPtr>& visitor)
{
    VisitObjectsMap(m_serversControllers, visitor);
}

void ClientController::OnServerConnected(const RemoteServices::IServicesSharedPtr& client)
{
    const auto serverController = RemoteServices::IServicesControllerSharedPtr{ RemoteServices::IServicesController::CreateRemote() };
    REMOTE_SERVICES_ASSERT(serverController);
    if (!serverController)
        return;

    if (!serverController->SetDataModel(client))
    {
        REMOTE_SERVICES_ASSERT(false);
        return;
    }

    m_serversControllers.emplace(client, serverController);
    NotifyListeners(&IClientControllerListener::OnServerConnected, serverController);

    serverController->CreateServices();
}

void ClientController::OnServerDisconnected(const RemoteServices::IServicesSharedPtr& client)
{
    const auto iterator = m_serversControllers.find(client);
    REMOTE_SERVICES_ASSERT(iterator != m_serversControllers.cend());
    if (iterator == m_serversControllers.cend())
        return;

    const auto& serverController = iterator->second;
    serverController->SetDataModel(nullptr);

    NotifyListeners(&IClientControllerListener::OnServerDisconnected, serverController);
    m_serversControllers.erase(iterator);
}

void ClientController::OnDataModelChanged()
{
    while (!m_serversControllers.empty())
    {
        const auto iterator = m_serversControllers.begin();
        if (!iterator->first || !iterator->second)
            m_serversControllers.erase(iterator);
        else
            OnServerDisconnected(iterator->first);
    }

    const auto& dataModel = GetDataModel();
    if (!dataModel)
        return;

    dataModel->VisitServers([this](const auto& client)
    {
        OnServerConnected(client);
    });
}

MVC::IListenerUniquePtr ClientController::CreateListener()
{
    return std::make_unique<ClientControllerListener>(*this);
}

IClientControllerUniquePtr IClientController::Create()
{
    return std::make_unique<ClientController>();
}