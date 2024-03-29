CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(RemoteServices)

SET(CMAKE_CXX_STANDARD 17)

SET(REMOTE_SERVICES_SOURCES
    Source/pch.h
    Source/pch.cpp
    
    Include/RemoteServices/RemoteServices.h
    
    Include/RemoteServices/Services/Service.h
    Include/RemoteServices/Services/ServiceFactory.h
    Include/RemoteServices/Services/ServiceFactory.inl
    Include/RemoteServices/Services/RequestResponseServiceBase.h
    Include/RemoteServices/Services/SingleConnectionRequestResponseServiceBase.h
    
    Include/RemoteServices/DataModels/Services.h
    Include/RemoteServices/DataModels/ServicesListenerBase.h
    Include/RemoteServices/DataModels/Server.h
    Include/RemoteServices/DataModels/ServerListenerBase.h
    Include/RemoteServices/DataModels/Client.h
    Include/RemoteServices/DataModels/ClientListenerBase.h
    
    Include/RemoteServices/Controllers/ServicesController.h
    Include/RemoteServices/Controllers/ServicesControllerListenerBase.h
    Include/RemoteServices/Controllers/ServerController.h
    Include/RemoteServices/Controllers/ServerControllerListenerBase.h
    Include/RemoteServices/Controllers/ClientController.h
    Include/RemoteServices/Controllers/ClientControllerListenerBase.h
    
    Source/RemoteServices/Services/Service.cpp
    Source/RemoteServices/Services/RequestResponseServiceBase.cpp
    Source/RemoteServices/Services/SingleConnectionRequestResponseServiceBase.cpp
    
    Source/RemoteServices/DataModels/Services.cpp
    Source/RemoteServices/DataModels/ServicesListenerBase.cpp
    Source/RemoteServices/DataModels/Server.cpp
    Source/RemoteServices/DataModels/ServerListenerBase.cpp
    Source/RemoteServices/DataModels/Client.cpp
    Source/RemoteServices/DataModels/ClientListenerBase.cpp
    
    Source/RemoteServices/Controllers/ServicesControllerBase.h
    Source/RemoteServices/Controllers/ServicesControllerBase.cpp
    Source/RemoteServices/Controllers/ServicesLocalController.cpp
    Source/RemoteServices/Controllers/ServicesRemoteController.cpp
    Source/RemoteServices/Controllers/ServicesControllerListenerBase.cpp
    Source/RemoteServices/Controllers/ServerController.cpp
    Source/RemoteServices/Controllers/ServerControllerListenerBase.cpp
    Source/RemoteServices/Controllers/ClientController.cpp
    Source/RemoteServices/Controllers/ClientControllerListenerBase.cpp
 
)

ADD_SUBDIRECTORY(Dependencies/MVCUtilities)
ADD_SUBDIRECTORY(Dependencies/Networking)

ADD_LIBRARY(RemoteServices STATIC ${REMOTE_SERVICES_SOURCES})

ADD_DEPENDENCIES(RemoteServices MVCUtilities Networking)

TARGET_INCLUDE_DIRECTORIES(RemoteServices PUBLIC Include Dependencies/MVCUtilities/Include Dependencies/Networking/Include)
TARGET_INCLUDE_DIRECTORIES(RemoteServices PRIVATE Source Source/RemoteServices)

TARGET_LINK_LIBRARIES(RemoteServices Networking)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)