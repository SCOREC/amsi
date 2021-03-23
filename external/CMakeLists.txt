set_directory_properties(PROPERTIES EXCLUDE_FROM_ALL YES)

include(FetchContent)
SET(CMAKE_CXX_FLAGS "")
FetchContent_Declare(ModelTraits
        GIT_REPOSITORY https://github.com/jacobmerson/model-traits
        GIT_TAG 96189186308ba8428a8d9d98b7154f4cd8495943
        )
FetchContent_MakeAvailable(ModelTraits)