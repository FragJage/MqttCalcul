#Activer le support c++11 
if(CMAKE_VERSION VERSION_LESS "3.1")
  set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
else ()
  set(CMAKE_CXX_STANDARD 11)
endif ()

#Librairie PTHREAD 
if(CMAKE_VERSION VERSION_GREATER "3.0")
  set(THREADS_PREFER_PTHREAD_FLAG ON)
endif ()
find_package(Threads)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated") # various warning flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage

include_directories(../dependency)
 
#Génération de la liste des fichiers sources
file(
  GLOB_RECURSE
  MqttBase_files
  ../dependency/MqttBase/*
)

file(
  GLOB_RECURSE
  Plateforms_files
  ../dependency/Plateforms/*
)

file(
  GLOB_RECURSE
  Service_files
  ../dependency/Service/*
)

file(
  GLOB_RECURSE
  SimpleFolders_files
  ../dependency/SimpleFolders/*
)

file(
  GLOB_RECURSE
  SimpleIni_files
  ../dependency/SimpleIni/*
)

file(
  GLOB_RECURSE
  SimpleLog_files
  ../dependency/SimpleLog/*
)

file(
  GLOB_RECURSE
  StringTools_files
  ../dependency/StringTools/*
)

file(
  GLOB_RECURSE
  UnitTest_files
  ../dependency/UnitTest/*
)

#Déclaration de l'exécutable
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
add_executable(
  unittest
  ${MqttBase_files}
  ${Plateforms_files}
  ${Service_files}
  ${SimpleFolders_files}
  ${SimpleIni_files}
  ${SimpleLog_files}
  ${StringTools_files}
  ${UnitTest_files}
  ../src/CacheManager.cpp
  ../src/Calculator.cpp
  ../src/CalculData.cpp
  ../src/MqttBridge.cpp
  ../src/MqttCalcul.cpp
  TestCalculator.cpp
  TestCalculData.cpp
  TestMqttCalcul.cpp
  main.cpp
)

target_link_libraries(unittest ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(unittest paho-mqtt3a)
target_link_libraries(unittest paho-mqttpp3)

#Spécification WIN32
if(WIN32)
  target_link_libraries(unittest ws2_32)
endif()
