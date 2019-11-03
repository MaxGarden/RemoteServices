#include "pch.h"
#include "RemoteServices/DataModels/ServerListenerBase.h"

using namespace RemoteServices;

void ServerListenerBase::OnClientConnected(const RemoteServices::IServicesSharedPtr&)
{
    //to override
}

void ServerListenerBase::OnClientDisconnected(const RemoteServices::IServicesSharedPtr&)
{
    //to override
}