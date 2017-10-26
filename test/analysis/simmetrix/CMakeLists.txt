add_subdirectory(encase)

add_executable(sms_to_vtu sms_to_vtu.cc)
target_link_libraries(sms_to_vtu analysis)

add_executable(sms_to_vtu_regions sms_to_vtu_regions.cc)
target_link_libraries(sms_to_vtu_regions analysis)
