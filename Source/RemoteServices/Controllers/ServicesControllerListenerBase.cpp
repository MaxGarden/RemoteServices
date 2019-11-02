#include "pch.h"
#include "Controllers/ServicesControllerListenerBase.h"

using namespace RemoteServices;

void ServicesControllerListenerBase::OnServicePaired(IService&)
{
    //to override
}

void ServicesControllerListenerBase::OnServiceUnparied(IService&)
{
    //to override
}