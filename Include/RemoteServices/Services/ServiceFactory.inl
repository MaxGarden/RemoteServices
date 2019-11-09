template<typename ServiceType>
ServiceFactory<ServiceType>::ServiceFactory(std::string&& serviceName) :
    m_serviceName{ std::move(serviceName) }
{
}

template<typename ServiceType>
const std::string& ServiceFactory<ServiceType>::GetServiceName() const noexcept
{
    return m_serviceName;
}

template<typename ServiceType>
IServiceUniquePtr ServiceFactory<ServiceType>::Create()
{
    return std::make_unique<ServiceType>();
}

template<typename ServiceType>
IServiceFactoryUniquePtr ServiceFactory<ServiceType>::CreateFactory(std::string&& serviceName)
{
    return std::make_unique<ServiceFactory<ServiceType>>(std::move(serviceName));
}