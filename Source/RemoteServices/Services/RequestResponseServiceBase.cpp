#include "pch.h"
#include "RemoteServices/Services/RequestResponseServiceBase.h"

using namespace RemoteServices;

const byte RequestResponseServiceBase::c_requestTag = 0;
const byte RequestResponseServiceBase::c_responseTag = 1;

bool RequestResponseServiceBase::Initialize()
{
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

    const auto tag = payload[0];
    const auto beginIterator = payload.cbegin() + sizeof(Response::ResponseType);
    const auto endIterator = payload.cend();
    
    auto servicePayload = ServicePayload{ beginIterator, endIterator };

    switch (tag)
    {
    case c_requestTag:
        return OnRequestReceived(connection, std::move(servicePayload));
    case c_responseTag:
        return OnResponseReceived(connection, std::move(servicePayload));
    default:
        REMOTE_SERVICES_ASSERT(false);
    }
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
    payload.insert(payload.begin(), c_requestTag);
    payload.insert(payload.begin(), request.Type);

    if (!connection->Send(std::move(payload)))
    {
        REMOTE_SERVICES_ASSERT(false);
        pendingRequests.back()(Response{ Response::ResponseType::Fail, {} });
        pendingRequests.pop_back();

        return false;
    }

    return true;
}

void RequestResponseServiceBase::OnRequestReceived(const IServiceConnectionSharedPtr& connection, ServicePayload&& payload)
{
    REMOTE_SERVICES_ASSERT(!payload.empty());
    if (payload.empty())
        return;

    const auto requestType = payload[0];
    const auto iterator = m_requestsHandlers.find(requestType);
    if (iterator == m_requestsHandlers.cend())
    {
        connection->Send({ c_responseTag, static_cast<byte>(Response::ResponseType::Fail) });
        return;
    }

    payload.erase(payload.begin());
    auto response = iterator->second(std::move(payload));
    auto& responseType = response.Type;

    REMOTE_SERVICES_ASSERT(responseType != Response::ResponseType::Unbind);
    if (responseType == Response::ResponseType::Unbind)
        responseType = Response::ResponseType::Fail;

    auto&& servicePayload = std::move(response.Payload);

    servicePayload.insert(servicePayload.begin(), { c_responseTag, static_cast<byte>(response.Type) });
    connection->Send(std::move(servicePayload));
}

void RequestResponseServiceBase::OnResponseReceived(const IServiceConnectionSharedPtr& connection, ServicePayload&& payload)
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
    payload.erase(payload.begin());

    responseCallback(Response{ responseType, std::move(payload)});
    pendingRequests.pop_front();
}