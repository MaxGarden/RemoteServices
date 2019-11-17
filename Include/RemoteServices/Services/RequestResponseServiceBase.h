#pragma once

#include "Service.h"

namespace RemoteServices
{

    class IResponseHandle
    {
    public:
        virtual ~IResponseHandle() = default;

        virtual bool IsValid() const noexcept = 0;
        virtual void Invalidate() = 0;
    };

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
                Unbind, //internal usage only

            } Type;

            ServicePayload Payload;
        };

        using RequestHandler = std::function<Response(ServicePayload&&)>;
        using ResponseCallback = std::function<void(Response&&)>;

    public:
        RequestResponseServiceBase() = default;
        virtual ~RequestResponseServiceBase() override = default;

        virtual bool Initialize() override;

        virtual void OnBind(const IServiceConnectionSharedPtr& connection) override;
        virtual void OnUnbind(const IServiceConnectionSharedPtr& connection) override;

        virtual void Finalize() override;

        virtual void OnReceived(const IServiceConnectionSharedPtr& connection, const ServicePayload& payload) override final;

    protected:
        bool RegisterRequestHandler(Request::RequestType request, RequestHandler&& requestHandler);
        bool UnregisterRequestHandler(Request::RequestType request);

        IResponseHandleSharedPtr SendRequest(const IServiceConnectionSharedPtr& connection, Request&& request, ResponseCallback&& responseCallback);

    private:
        void OnRequestReceived(const IServiceConnectionSharedPtr& connection, ServicePayload&& payload);
        void OnResponseReceived(const IServiceConnectionSharedPtr& connection, ServicePayload&& payload);

    private:
        std::map<Request::RequestType, RequestHandler> m_requestsHandlers;
        std::map<IServiceConnectionSharedPtr, std::deque<IResponseHandleSharedPtr>> m_pendingRequests;

        static const byte c_requestTag;
        static const byte c_responseTag;
    };
}