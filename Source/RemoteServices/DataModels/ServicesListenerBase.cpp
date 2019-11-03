#include "pch.h"
#include "RemoteServices/DataModels/ServicesListenerBase.h"

using namespace RemoteServices;

void ServicesListenerBase::OnReceived(const Packet&)
{
    //to override
}

void ServicesListenerBase::OnDisconnected()
{
    //to override
}