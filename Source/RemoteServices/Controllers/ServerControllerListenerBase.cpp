#include "pch.h"
#include "RemoteServices/Controllers/ServerControllerListenerBase.h"

using namespace RemoteServices;

void ServerControllerListenerBase::OnClientConnected(const RemoteServices::IServicesControllerSharedPtr&)
{
    //to override
}

void ServerControllerListenerBase::OnClientDisconnected(const RemoteServices::IServicesControllerSharedPtr&)
{
    //to override
}