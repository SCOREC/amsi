set(control_public_headers
  amsi.h ControlService.h Migration.h)

set(control_source_files
  amsi.cc ControlService.cc Migration.cc )

add_library(amsi_control ${control_source_files})

target_include_directories(amsi_control PUBLIC
					$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
					$<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(amsi_control PUBLIC amsi_util amsi_meta amsi_interface)

set_property(TARGET amsi_control PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS amsi_control
	EXPORT	amsicontrolTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	INCLUDES DESTINATION include/amsi)

install(EXPORT amsicontrolTargets
	DESTINATION ${ConfigPackageLocation})

install(FILES ${control_public_headers} DESTINATION include/amsi)
