template<typename ServiceType>
ClientServiceFactory<ServiceType>::ClientServiceFactory(std::string&& serviceName) :
    m_serviceName(std::move(serviceName))
{
}

template<typename ServiceType>
const std::string& ClientServiceFactory<ServiceType>::GetServiceName() const noexcept
{
    return m_serviceName;
}

template<typename ServiceType>
IServiceUniquePtr ClientServiceFactory<ServiceType>::Create()
{
    return std::make_unique<ServiceType>();
}