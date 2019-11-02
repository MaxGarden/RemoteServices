#include "Controllers/ServicesController.h"
#include "MVC/ControllerBase.h"

namespace RemoteServices
{
    class ServicesControllerBase : public MVC::ControllerBase<IServicesController, IServices, IServicesControllerListener>
    {
    public:
        ServicesControllerBase() = default;
        virtual ~ServicesControllerBase() override = default;

        virtual bool RegisterServiceFactory(IServiceFactoryUniquePtr&& serviceFactory) override final;
        virtual void VisitServices(const MVC::ConstVisitorType<IService>& visitor) const override final;

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

        virtual void PairService(const std::string& serviceName, IServiceUniquePtr&& service, byte servicePort);
        virtual void UnpairService(ServiceData& serviceData);

        virtual void UnpairServices();

        void VisitServiceFactories(const MVC::VisitorType<IServiceFactory&>& visitor);

        std::optional<byte> GetFreeServicePort() const noexcept;

    protected:
        static const byte s_pairServicePort = 0;

    private:
        std::vector<IServiceFactoryUniquePtr> m_servicesFactories;

        std::map<byte, ServiceData> m_pairedServices;
        bool m_isConnected = false;
    };
}