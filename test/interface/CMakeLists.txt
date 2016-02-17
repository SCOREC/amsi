set(MODEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/models")
set(MESH_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/meshes")

message(STATUS "Test models located in ${MODEL_DIR}")
message(STATUS "Test meshes located in ${MESH_DIR}")

add_subdirectory(init)
add_subdirectory(loading)
add_subdirectory(sim_attributes)
add_subdirectory(boundary conditions)
add_subdirectory(linear_elasticity)
add_subdirectory(nonlinear_elasticity)
add_subdirectory(partitioned_linear_elastic)
add_subdirectory(mesh_adaptivity)
