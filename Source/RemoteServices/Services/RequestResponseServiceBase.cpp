#include "pch.h"
#include "RemoteServices/Services/RequestResponseServiceBase.h"

using namespace RemoteServices;

bool RequestResponseServiceBase::Initialize()
{
    OnRegisterRequestsHandlers();
    return true;
}

void RequestResponseServiceBase::OnBind(const IServiceConnectionSharedPtr& connection)
{
    m_pendingRequests.emplace(connection, std::deque<ResponseCallback>{});
}

void RequestResponseServiceBase::OnUnbind(const IServiceConnectionSharedPtr& connection)
{
    const auto iterator = m_pendingRequests.find(connection);
    REMOTE_SERVICES_ASSERT(iterator != m_pendingRequests.cend());
    if (iterator == m_pendingRequests.cend())
        return;

    auto& pendingRequests = iterator->second;
    while (!pendingRequests.empty())
    {
        const auto& responseCallback = pendingRequests.front();
        responseCallback(Response{ Response::ResponseType::Unbind, ServicePayload{} });
        pendingRequests.pop_front();
    }

    m_pendingRequests.erase(iterator);
}

void RequestResponseServiceBase::Finalize()
{
    while (!m_requestsHandlers.empty())
        UnregisterRequestHandler(m_requestsHandlers.begin()->first);
}

void RequestResponseServiceBase::OnReceived(const IServiceConnectionSharedPtr& connection, const ServicePayload& payload)
{
    REMOTE_SERVICES_ASSERT(!payload.empty());
    if (payload.empty())
        return;

    const auto iterator = m_pendingRequests.find(connection);
    REMOTE_SERVICES_ASSERT(iterator != m_pendingRequests.cend());
    if (iterator == m_pendingRequests.cend())
        return;

    auto& pendingRequests = iterator->second;
    const auto& responseCallback = pendingRequests.front();

    const auto responseType = static_cast<Response::ResponseType>(payload[0]);
    const auto beginIterator = payload.cbegin() + sizeof(Response::ResponseType);
    const auto endIterator = payload.cend();

    responseCallback(Response{ responseType, ServicePayload{ beginIterator, endIterator } });
    pendingRequests.pop_front();
}

bool RequestResponseServiceBase::RegisterRequestHandler(Request::RequestType request, RequestHandler&& requestHandler)
{
    const auto iterator = m_requestsHandlers.find(request);
    REMOTE_SERVICES_ASSERT(iterator == m_requestsHandlers.cend());
    if (iterator != m_requestsHandlers.cend())
        return false;

    m_requestsHandlers.emplace(request, std::move(requestHandler));
    return false;
}

bool RequestResponseServiceBase::UnregisterRequestHandler(Request::RequestType request)
{
    const auto iterator = m_requestsHandlers.find(request);
    REMOTE_SERVICES_ASSERT(iterator != m_requestsHandlers.cend());
    if (iterator == m_requestsHandlers.cend())
        return false;

    m_requestsHandlers.erase(iterator);
    return true;
}

bool RequestResponseServiceBase::SendRequest(const IServiceConnectionSharedPtr& connection, Request&& request, ResponseCallback&& responseCallback)
{
    const auto iterator = m_pendingRequests.find(connection);
    REMOTE_SERVICES_ASSERT(iterator != m_pendingRequests.cend());
    if (iterator == m_pendingRequests.cend())
        return false;

    auto& pendingRequests = iterator->second;
    pendingRequests.emplace_back(std::move(responseCallback));

    auto&& payload = std::move(request.Payload);
    payload.insert(payload.begin(), request.Type);

    connection->Send(std::move(payload));
}