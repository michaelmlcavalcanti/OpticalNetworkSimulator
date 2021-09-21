cmake_minimum_required(VERSION 3.17)
project(OpticalNetworkSimulator)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        main.cpp
        src/Kernel.cpp
        src/Algorithms/GA/CoreOrderIndividual.cpp
        src/Algorithms/GA/GA.cpp
        src/Algorithms/GA/
        src/Algorithms/GA/GA_MO.cpp
        src/Algorithms/GA/GA_NumInterRoutesMSCL.cpp
        src/Algorithms/GA/GA_RsaOrder.cpp
        src/Algorithms/GA/GA_SO.cpp
        src/Algorithms/GA/GACoreOrder.cpp
        src/Algorithms/GA/GA_PDPPBO.cpp
        src/Algorithms/GA/Individual.cpp
        src/Algorithms/GA/IndividualBool.cpp
        src/Algorithms/GA/IndividualNumRoutesMSCL.cpp
        src/Algorithms/GA/IndividualPDPPBO.cpp
        src/Algorithms/PSO/ParticlePSO.cpp
        src/Algorithms/PSO/ParticlePSO_SCRA.cpp
        src/Algorithms/PSO/ParticlePSO_SCRA2.cpp
        src/Algorithms/PSO/PSO.cpp
        src/Calls/Call.cpp
        src/Calls/CallDevices.cpp
        src/Calls/Event.cpp
        src/Calls/EventGenerator.cpp
        src/Calls/Traffic.cpp
        src/Data/Data.cpp
        src/Data/InputOutput.cpp
        src/Data/Options.cpp
        src/Data/Parameters.cpp
        src/GeneralClasses/Def.cpp
        src/GeneralClasses/General.cpp
        src/ResourceAllocation/CSA.cpp
        src/ResourceAllocation/Modulation.cpp
        src/ResourceAllocation/ResourceAlloc.cpp
        src/ResourceAllocation/ResourceDeviceAlloc.cpp
        src/ResourceAllocation/Resources.cpp
        src/ResourceAllocation/Route.cpp
        src/ResourceAllocation/Routing.cpp
        src/ResourceAllocation/SA.cpp
        src/ResourceAllocation/CSA.cpp
        src/ResourceAllocation/Signal.cpp
        src/ResourceAllocation/ProtectionSchemes/DedicatedPathProtection.cpp
        src/ResourceAllocation/ProtectionSchemes/PartitioningDedicatedPathProtection.cpp
        src/ResourceAllocation/ProtectionSchemes/ProtectionScheme.cpp
        src/ResourceAllocation/RegeneratorAssignment/DRE2BR.cpp
        src/ResourceAllocation/RegeneratorAssignment/FirstLongestReach.cpp
        src/ResourceAllocation/RegeneratorAssignment/FirstNarrowestSpectrum.cpp
        src/ResourceAllocation/RegeneratorAssignment/MaximumRegeneration.cpp
        src/ResourceAllocation/RegeneratorAssignment/MinimumRegeneration.cpp
        src/ResourceAllocation/RegeneratorAssignment/MinimumSlotsMaxReg.cpp
        src/ResourceAllocation/RegeneratorAssignment/MinimumSlotsMinReg.cpp
        src/ResourceAllocation/RegeneratorAssignment/RegeneratorAssignment.cpp
        src/ResourceAllocation/RegeneratorAssignment/SCRA.cpp
        src/ResourceAllocation/RegeneratorAssignment/SCRA2.cpp
        src/SimulationType/SimulationGA.cpp
        src/SimulationType/SimulationMultiLoad.cpp
        src/SimulationType/SimulationMultiNumDevices.cpp
        src/SimulationType/SimulationPSO.cpp
        src/SimulationType/SimulationType.cpp
        src/Structure/Core.cpp
        src/Structure/Link.cpp
        src/Structure/Node.cpp
        src/Structure/NodeDevices.cpp
        src/Structure/Topology.cpp
        src/Structure/Devices/BVT.cpp
        src/Structure/Devices/Device.cpp
        src/Structure/Devices/Regenerator.cpp
        src/Structure/Devices/SBVT_TSS_ML.cpp)

add_executable(OpticalNetworkSimulator ${SOURCE_FILES})

INCLUDE_DIRECTORIES(include)