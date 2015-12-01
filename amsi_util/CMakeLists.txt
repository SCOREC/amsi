set(util_public_headers
  amsiUtil.h
  amsiUtilConfig.h
  amsiHost.h
  amsiReporter.h
  amsiUtilFunctions.h
  amsiMPI.h
  amsiByteStream.h
  unordered_map.h 
  pystring.h
  )

set(util_source_files
  amsiUtil.cc
  amsiMPI.cc
  amsiReporter.cc
  pystring.cc
  )

find_package(Core COMPONENTS Sim)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h.in 
               ${CMAKE_CURRENT_SOURCE_DIR}/amsiUtilConfig.h 
	       @ONLY)

add_library(amsi_util ${util_source_files})
set_property(TARGET amsi_util PROPERTY VERSION ${AMSI_VERSION})

target_include_directories(amsi_util PUBLIC
				     $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
				     $<INSTALL_INTERFACE:include/amsi>)

if(CORE_FOUND)
  target_include_directories(amsi_util PUBLIC ${CORE_INCLUDE_DIRS})
  target_link_libraries(amsi_util PUBLIC ${CORE_LIBRARIES})
endif()

if($ENV{HOST} STREQUAL "q.ccni.rpi.edu")
  target_include_directories(amsi_util PUBLIC ${BOOST_INCLUDE_DIR})
endif()

install(TARGETS amsi_util 
	EXPORT amsiutilTargets 
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsiutilTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${util_public_headers} DESTINATION include/amsi)
