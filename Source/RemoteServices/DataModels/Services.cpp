#include "pch.h"
#include "RemoteServices/DataModels/Services.h"
#include "MVC/ListenableBase.h"
#include "Networking/Connection.h"

using namespace RemoteServices;

using PacketSizeType = size_t;

class Services final : public MVC::ListenableBase<IServices, IServicesListener>
{
public:
    Services() = default;
    virtual ~Services() override final = default;

    bool SetConnection(const Networking::IConnectionSharedPtr& connection);

    virtual bool IsConnected() const noexcept override final;
    virtual bool Send(Packet&& packet) override final;

    virtual std::optional<std::string> GetRemoteAddress() const noexcept override final;

    virtual bool Disconnect() override final;

private:
    Networking::IConnectionSharedPtr m_connection;
};

bool Services::SetConnection(const Networking::IConnectionSharedPtr& connection)
{
    REMOTE_SERVICES_ASSERT(!m_connection);
    if (!connection || !connection->IsConnected())
        return false;
    
    m_connection = connection;

    m_connection->SetOnClosedCallback([this]()
    {
        NotifyListeners(&IServicesListener::OnDisconnected);
    });

    m_connection->SetOnReceivedCallback([this](const auto& payload)
    {
        if (payload.empty())
            return;

        const auto servicePort = payload.front();
        const auto servicePayload = ServicePayload(payload.cbegin() + 1, payload.cend());
        NotifyListeners(&IServicesListener::OnReceived, Packet{ servicePort, servicePayload });
    });

    return true;
}

bool Services::IsConnected() const noexcept
{
    return m_connection ? m_connection->IsConnected() : false;
}

bool Services::Send(Packet&& packet)
{
    const auto isConnected = IsConnected();
    REMOTE_SERVICES_ASSERT(isConnected);
    if (!isConnected)
        return false;

    const auto packetRawSize = sizeof(byte) + packet.Payload.size();

    if (packetRawSize > std::numeric_limits<PacketSizeType>::max())
        return false;

    const auto packetSize = static_cast<PacketSizeType>(packetRawSize);
    const auto expectedSentDataSize = packetSize + sizeof(PacketSizeType);

    auto&& payload = std::move(packet.Payload);
    packet.Payload.insert(payload.begin(), packet.ServicePort);

    const auto result = m_connection->Send(std::move(payload));

    REMOTE_SERVICES_ASSERT(result);
    return result;
}

std::optional<std::string> Services::GetRemoteAddress() const noexcept
{
    return m_connection ? std::make_optional(m_connection->GetAddress()) : std::nullopt;
}

bool Services::Disconnect()
{
    const auto isConnected = IsConnected();
    REMOTE_SERVICES_ASSERT(isConnected);
    if (!isConnected)
        return false;

    REMOTE_SERVICES_ASSERT(m_connection);
    if (!m_connection)
        return false;

    m_connection->Close();
    return true;
}

IServicesUniquePtr IServices::Create(const Networking::IConnectionSharedPtr& connection)
{
    auto result = std::make_unique<Services>();
    if (!result->SetConnection(connection))
        return nullptr;

    return result;
}