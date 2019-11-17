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

IResponseHandleSharedPtr SingleConnectionRequestResponseServiceBase::SendRequest(Request&& request, ResponseCallback&& responseCallback)
{
    REMOTE_SERVICES_ASSERT(m_connection);
    if (!m_connection)
        return nullptr;

    REMOTE_SERVICES_ASSERT(responseCallback);
    if (!responseCallback)
        return nullptr;

    return RequestResponseServiceBase::SendRequest(m_connection, std::move(request), std::move(responseCallback));
}