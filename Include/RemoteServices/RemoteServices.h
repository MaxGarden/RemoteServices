#pragma once

#include <string>
#include <vector>
#include <memory>
#include <cstring>

namespace RemoteServices
{
    using byte = unsigned char;

    #define REMOTE_SERVICES_ASSERT(expression) assert(expression)

    #define DECLARE_POINTERS(x)                                 \
        class x;                                                \
        using x##SharedPtr = std::shared_ptr<x>;                \
        using x##ConstSharedPtr = std::shared_ptr<const x>;     \
        using x##WeakPtr = std::weak_ptr<x>;                    \
        using x##ConstWeakPtr = std::weak_ptr<const x>;         \
        using x##UniquePtr = std::unique_ptr<x>;                \
        using x##ConstUniquePtr = std::unique_ptr<const x>;     \

    DECLARE_POINTERS(IService);
    DECLARE_POINTERS(IServiceConnection);
    DECLARE_POINTERS(IServiceFactory);
    DECLARE_POINTERS(IServices);
    DECLARE_POINTERS(IServicesController);
    DECLARE_POINTERS(IServicesBuilder);
    DECLARE_POINTERS(IServicesBuildersProvider);
}