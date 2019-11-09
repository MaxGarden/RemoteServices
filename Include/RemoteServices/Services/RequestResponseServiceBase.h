#pragma once

#include "Service.h"

namespace RemoteServices
{
    class RequestResponseServiceBase : public IService
    {
    public:
        
        struct Request
        {
            using RequestType = byte;

            RequestType Type;
            ServicePayload Payload;
        };

        struct Response
        {
            enum class ResponseType : byte
            {
                Ok,
                Fail,
                Unbind //internal usage only

            } Type;

            ServicePayload Payload;
        };


        using RequestHandler = std::function<Response(ServicePayload&&)>;
        using ResponseCallback = std::function<void(Response&&)>;

    public:
        RequestResponseServiceBase() = default;
        virtual ~RequestResponseServiceBase() override = default;

        virtual bool Initialize() override;

        virtual void OnBind(const IServiceConnectionSharedPtr& connection) override final;
        virtual void OnUnbind(const IServiceConnectionSharedPtr& connection) override final;

        virtual void Finalize() override;

        virtual void OnReceived(const IServiceConnectionSharedPtr& connection, const ServicePayload& payload) override final;

    protected:
        virtual void OnRegisterRequestsHandlers() = 0;

        bool RegisterRequestHandler(Request::RequestType request, RequestHandler&& requestHandler);
        bool UnregisterRequestHandler(Request::RequestType request);

        bool SendRequest(const IServiceConnectionSharedPtr& connection, Request&& request, ResponseCallback&& responseCallback);

    private:
        std::map<Request::RequestType, RequestHandler> m_requestsHandlers;
        std::map<IServiceConnectionSharedPtr, std::deque<ResponseCallback>> m_pendingRequests;
    };
}