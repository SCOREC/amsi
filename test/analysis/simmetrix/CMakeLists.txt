add_subdirectory(encase)

add_executable(sms_to_vtu sms_to_vtu.cc)
target_link_libraries(sms_to_vtu analysis)

