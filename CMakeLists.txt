CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(RemoteServices)

SET(CMAKE_CXX_STANDARD 17)

SET(REMOTE_SERVICES_SOURCES
    Source/pch.h
    Source/pch.cpp
    
    Include/RemoteServices/RemoteServices.h
    Include/RemoteServices/Service.h
    Include/RemoteServices/ServicesBuilder.h
    Include/RemoteServices/DataModels/Services.h
    Include/RemoteServices/DataModels/ServicesListenerBase.h
    Include/RemoteServices/Controllers/ServicesController.h
    Include/RemoteServices/Controllers/ServicesControllerListenerBase.h
    
    Source/RemoteServices/Service.cpp
    Source/RemoteServices/ServicesBuilder.cpp
    Source/RemoteServices/DataModels/Services.cpp
    Source/RemoteServices/DataModels/ServicesListenerBase.cpp
    Source/RemoteServices/Controllers/ServicesControllerBase.h
    Source/RemoteServices/Controllers/ServicesControllerBase.cpp
    Source/RemoteServices/Controllers/ServicesLocalController.cpp
    Source/RemoteServices/Controllers/ServicesRemoteController.cpp
    Source/RemoteServices/Controllers/ServicesControllerListenerBase.cpp
 
)

ADD_SUBDIRECTORY(Dependencies/MVCUtilities)
ADD_SUBDIRECTORY(Dependencies/Networking)

ADD_LIBRARY(RemoteServices STATIC ${REMOTE_SERVICES_SOURCES})

ADD_DEPENDENCIES(RemoteServices MVCUtilities Networking)

TARGET_INCLUDE_DIRECTORIES(RemoteServices PUBLIC Include Dependencies/MVCUtilities/Include Dependencies/Networking/Include)
TARGET_INCLUDE_DIRECTORIES(RemoteServices PRIVATE Source Source/RemoteServices Include/RemoteServices)

TARGET_LINK_LIBRARIES(RemoteServices MVCUtilities)
TARGET_LINK_LIBRARIES(RemoteServices Networking)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)