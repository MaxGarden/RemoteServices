#include "pch.h"
#include "RemoteServices/Controllers/ServicesControllerListenerBase.h"

using namespace RemoteServices;

void ServicesControllerListenerBase::OnServicePaired(IService&)
{
    //to override
}

void ServicesControllerListenerBase::OnServiceUnparied(IService&)
{
    //to override
}