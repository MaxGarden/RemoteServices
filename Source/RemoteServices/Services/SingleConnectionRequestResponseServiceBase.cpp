#include "pch.h"
#include "RemoteServices/Services/SingleConnectionRequestResponseServiceBase.h"

using namespace RemoteServices;

bool SingleConnectionRequestResponseServiceBase::Initialize()
{
    //to override
    return true;
}

void SingleConnectionRequestResponseServiceBase::OnBind(const IServiceConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(!m_connection);
    if (m_connection)
        return;

    m_connection = connection;
}

void SingleConnectionRequestResponseServiceBase::OnUnbind(const IServiceConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(m_connection == connection);
    if (m_connection == connection)
        m_connection.reset();
}

void SingleConnectionRequestResponseServiceBase::Finalize()
{
    //to override
}

bool SingleConnectionRequestResponseServiceBase::SendRequest(Request&& request, ResponseCallback&& responseCallback)
{
    REMOTE_SERVICES_ASSERT(m_connection);
    if (!m_connection)
        return false;

    REMOTE_SERVICES_ASSERT(responseCallback);
    if (!responseCallback)
        return false;

    return RequestResponseServiceBase::SendRequest(m_connection, std::move(request), std::move(responseCallback));
}