#include "pch.h"
#include "RemoteServices/Controllers/ClientControllerListenerBase.h"

using namespace RemoteServices;

void ClientControllerListenerBase::OnServerConnected(const RemoteServices::IServicesControllerSharedPtr&)
{
    //to override
}

void ClientControllerListenerBase::OnServerDisconnected(const RemoteServices::IServicesControllerSharedPtr&)
{
    //to override
}