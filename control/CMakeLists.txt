set(control_public_headers
  amsi.h
  amsiControl.h
  amsiControlService.h
  amsiControlService_impl.h
  Migration.h)

set(control_source_files
  amsi.cc
  amsiControl.cc
  amsiControlService.cc
  Migration.cc )

find_package(SCOREC CONFIG REQUIRED)

add_library(control ${control_source_files})

target_include_directories(control PUBLIC
                                   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                                   $<INSTALL_INTERFACE:include/amsi>)

target_link_libraries(control PUBLIC util meta SCOREC::pcu)

set_property(TARGET control PROPERTY VERSION ${AMSI_VERSION})

install(TARGETS control
        EXPORT  amsicontrolTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        INCLUDES DESTINATION include/amsi)

install(EXPORT amsicontrolTargets
        DESTINATION ${ConfigPackageLocation})

install(FILES ${control_public_headers} DESTINATION include/amsi)
