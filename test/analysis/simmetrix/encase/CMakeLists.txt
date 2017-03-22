if(TARGET_SUPPORTS_SHARED_LIBS)
  add_executable(encase main.cc)
  target_link_libraries(encase analysis)
endif()
