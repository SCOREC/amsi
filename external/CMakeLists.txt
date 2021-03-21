set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL YES)

include(FetchContent)
SET(CMAKE_CXX_FLAGS "")
FetchContent_Declare(ModelTraits
        GIT_REPOSITORY https://github.com/jacobmerson/model-traits
        GIT_TAG 5820879a563be522e980191b05350f5d2e4a9793
        )
FetchContent_MakeAvailable(ModelTraits)