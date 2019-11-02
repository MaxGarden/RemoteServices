CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
PROJECT(RemoteServices)

SET(REMOTE_SERVICES_SOURCES
    Source/pch.h
    Source/pch.cpp
)

ADD_SUBDIRECTORY(Dependencies/MVCUtilities)
ADD_SUBDIRECTORY(Dependencies/Networking)

ADD_LIBRARY(RemoteServices STATIC ${REMOTE_SERVICES_SOURCES})

ADD_DEPENDENCIES(RemoteServices MVCUtilities Networking)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)