#include "pch.h"
#include "RemoteServices/ServicesBuilder.h"

using namespace RemoteServices;

class ServicesBuilderProvider final : public IServicesBuildersProvider
{
public:
    ServicesBuilderProvider() = default;
    virtual ~ServicesBuilderProvider() override final = default;

    virtual bool RegisterBuilder(byte deviceId, IServicesBuilderUniquePtr&& builder) override final;
    virtual bool UnregisterBuilder(byte deviceId) override final;

    virtual IServicesBuilder* GetBuilder(byte deviceId) const noexcept override final;

private:
    std::map<byte, IServicesBuilderUniquePtr> m_servicesBuilders;
};

bool ServicesBuilderProvider::RegisterBuilder(byte deviceId, IServicesBuilderUniquePtr&& builder)
{
    const auto iterator = m_servicesBuilders.find(deviceId);
    REMOTE_SERVICES_ASSERT(iterator == m_servicesBuilders.cend());
    if (iterator != m_servicesBuilders.cend())
        return false;

    m_servicesBuilders.emplace(deviceId, std::move(builder));
    return true;
}

bool ServicesBuilderProvider::UnregisterBuilder(byte deviceId)
{
    const auto result = m_servicesBuilders.erase(deviceId) == 1;
    REMOTE_SERVICES_ASSERT(result);
    return result;
}

IServicesBuilder* ServicesBuilderProvider::GetBuilder(byte deviceId) const noexcept
{
    const auto iterator = m_servicesBuilders.find(deviceId);
    REMOTE_SERVICES_ASSERT(iterator != m_servicesBuilders.cend());
    if (iterator == m_servicesBuilders.cend())
        return nullptr;

    return iterator->second.get();
}

IServicesBuildersProviderUniquePtr IServicesBuildersProvider::Create()
{
    return std::make_unique<ServicesBuilderProvider>();
}