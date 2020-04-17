
set(TBB_DIR ${PROJECT_SOURCE_DIR}/vendor/)
include(${TBB_DIR}/cmake/TBBBuild.cmake)

# Build Intel TBB with enabled Community Preview Features (CPF).
tbb_build(TBB_ROOT ${TBB_DIR} CONFIG_DIR TBB_DIR MAKE_ARGS tbb_cpf=1)

find_package(TBB REQUIRED tbb_preview)

# Link Intel TBB imported targets to the executable;
# "TBB::tbb_preview" can be used instead of "${TBB_IMPORTED_TARGETS}".
add_executable(hellofib hellofib.cpp)
target_link_libraries(hellofib ${TBB_IMPORTED_TARGETS})
