#include "pch.h"
#include "ServerControllerListenerBase.h"

using namespace RemoteServices;

void ServerControllerListenerBase::OnClientConnected(const RemoteServices::IServicesControllerSharedPtr&)
{
    //to override
}

void ServerControllerListenerBase::OnClientDisconnected(const RemoteServices::IServicesControllerSharedPtr&)
{
    //to override
}