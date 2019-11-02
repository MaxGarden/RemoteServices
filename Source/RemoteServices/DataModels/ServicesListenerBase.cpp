#include "pch.h"
#include "DataModels/ServicesListenerBase.h"

using namespace RemoteServices;

void ServicesListenerBase::OnReceived(const Packet&)
{
    //to override
}

void ServicesListenerBase::OnDisconnected()
{
    //to override
}