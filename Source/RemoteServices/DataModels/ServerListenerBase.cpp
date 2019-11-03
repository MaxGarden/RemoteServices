#include "pch.h"
#include "ServerListenerBase.h"

using namespace RemoteServices;

void ServerListenerBase::OnClientConnected(const RemoteServices::IServicesSharedPtr&)
{
    //to override
}

void ServerListenerBase::OnClientDisconnected(const RemoteServices::IServicesSharedPtr&)
{
    //to override
}