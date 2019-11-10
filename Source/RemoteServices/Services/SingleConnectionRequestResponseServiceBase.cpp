#include "pch.h"
#include "RemoteServices/Services/SingleConnectionRequestResponseServiceBase.h"

using namespace RemoteServices;

void SingleConnectionRequestResponseServiceBase::OnBind(const IServiceConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(!m_connection);
    if (m_connection)
        return;

    m_connection = connection;
    RequestResponseServiceBase::OnBind(connection);
}

void SingleConnectionRequestResponseServiceBase::OnUnbind(const IServiceConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(m_connection == connection);
    if (m_connection != connection)
        return;

    RequestResponseServiceBase::OnUnbind(connection);
    m_connection.reset();
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