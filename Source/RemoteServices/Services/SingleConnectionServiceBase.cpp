#include "pch.h"
#include "RemoteServices/Services/SingleConnectionServiceBase.h"

using namespace RemoteServices;

bool SignaleConnectionServiceBase::Initialize()
{
    //to override
    return true;
}

void SignaleConnectionServiceBase::OnBind(const IServiceConnectionSharedPtr& connection)
{
    DEBUG_ASSERT(!m_Connection);
    if (m_Connection)
        return;

    m_Connection = connection;
}

void SignaleConnectionServiceBase::OnUnbind(const IServiceConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(m_Connection == connection);
    if (m_Connection == connection)
        m_Connection.reset();
}

void SignaleConnectionServiceBase::Finalize()
{
    //to override
}

void SignaleConnectionServiceBase::OnReceived(const ServicePayload&)
{
    //to override
}

bool SignaleConnectionServiceBase::Send(ServicePayload&& payload)
{
    const auto& connection = GetConnection();
    REMOTE_SERVICES_ASSERT(connection);
    if (!connection)
        return false;

    const auto result = connection->Send(std::move(payload));
    REMOTE_SERVICES_ASSERT(result);
    return result;
}

const IServiceConnectionSharedPtr& SignaleConnectionServiceBase::GetConnection() const noexcept
{
    return m_Connection;
}