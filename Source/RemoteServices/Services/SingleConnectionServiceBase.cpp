#include "pch.h"
#include "RemoteServices/Services/SingleConnectionServiceBase.h"

using namespace RemoteServices;

bool SingleConnectionServiceBase::Initialize()
{
    //to override
    return true;
}

void SingleConnectionServiceBase::OnBind(const IServiceConnectionSharedPtr& connection)
{
    DEBUG_ASSERT(!m_Connection);
    if (m_Connection)
        return;

    m_Connection = connection;
}

void SingleConnectionServiceBase::OnUnbind(const IServiceConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(m_Connection == connection);
    if (m_Connection == connection)
        m_Connection.reset();
}

void SingleConnectionServiceBase::Finalize()
{
    //to override
}

void SingleConnectionServiceBase::OnReceived(const ServicePayload&)
{
    //to override
}

bool SingleConnectionServiceBase::Send(ServicePayload&& payload)
{
    const auto& connection = GetConnection();
    REMOTE_SERVICES_ASSERT(connection);
    if (!connection)
        return false;

    const auto result = connection->Send(std::move(payload));
    REMOTE_SERVICES_ASSERT(result);
    return result;
}

const IServiceConnectionSharedPtr& SingleConnectionServiceBase::GetConnection() const noexcept
{
    return m_Connection;
}