#include "RemoteServices/Controllers/ServicesController.h"
#include "MVC/ControllerBase.h"

namespace RemoteServices
{
    class ServicesControllerBase : public MVC::ControllerBase<IServicesController, IServices, IServicesControllerListener>
    {
    public:
        ServicesControllerBase() = default;
        virtual ~ServicesControllerBase() override = default;

        virtual bool RegisterServiceFactory(IServiceFactoryUniquePtr&& serviceFactory) override final;
        virtual void VisitServices(const MVC::VisitorType<IService>& visitor) const override final;

        virtual std::string GetRemoteAddress() const noexcept override final;

        virtual bool Disconnect() override final;

        bool Send(byte servicePort, ServicePayload&& payload);

        void OnReceived(const Packet& packet);

        void OnConnected();
        void OnDisconnected();

    protected:
        struct ServiceData
        {
            byte ServicePort;
            std::string ServiceName;
            IServiceUniquePtr Service;
            IServiceConnectionSharedPtr Connection;
        };

    protected:
        virtual void OnDataModelChanged() override final;
        virtual MVC::IListenerUniquePtr CreateListener() override final;

        virtual void OnPairMessage(const ServicePayload& payload) = 0;

        void PairService(const std::string& serviceName, IServiceUniquePtr&& service, byte servicePort);
        void UnpairService(ServiceData& serviceData);

        virtual void UnpairServices();

        void VisitServiceFactories(const MVC::VisitorType<IServiceFactory&>& visitor);

        std::optional<byte> GetFreeServicePort() const noexcept;

    protected:
        virtual void OnServiceUnpaired(ServiceData& serviceData);

    protected:
        static const byte s_pairServicePort;

    private:
        std::vector<IServiceFactoryUniquePtr> m_servicesFactories;

        std::map<byte, ServiceData> m_pairedServices;
        bool m_isConnected = false;
    };
}