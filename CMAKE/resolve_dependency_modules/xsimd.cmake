include_guard(GLOBAL)

message(STATUS "Building xsimd from source")
FetchContent_Declare(
        xsimd
        GIT_REPOSITORY https://github.com/xtensor-stack/xsimd.git
        GIT_TAG e12bf0a928bd6668ff701db55803a9e316cb386c # 10.0.0
        GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(xsimd)