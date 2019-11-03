#include "pch.h"
#include "RemoteServices/DataModels/ClientListenerBase.h"

using namespace RemoteServices;

void ClientListenerBase::OnServerConnected(const RemoteServices::IServicesSharedPtr&)
{
    //to override
}

void ClientListenerBase::OnServerDisconnected(const RemoteServices::IServicesSharedPtr&)
{
    //to override
}