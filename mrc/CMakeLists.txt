project(mrc
	VERSION ${MR_VERSION_MAJOR}.${MR_VERSION_MINOR}.${MR_VERSION_PATCH}
	LANGUAGES C CXX)


include(FetchContent)

FetchContent_Declare(
  llvm-project
  GIT_REPOSITORY https://github.com/llvm/llvm-project.git
  GIT_TAG llvmorg-20.1.5
  GIT_SHALLOW TRUE
  GIT_PROGRESS TRUE
  SOURCE_SUBDIR llvm
)
FetchContent_MakeAvailable(llvm-project)

add_executable(mrc Main.cpp Lexer.cpp AST.cpp)
target_include_directories(mrc PRIVATE
  ${llvm-project_SOURCE_DIR}/llvm/include
  ${llvm-project_BINARY_DIR}/include
)
target_link_libraries(mrc LLVM)
