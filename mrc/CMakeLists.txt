project(mrc
	VERSION ${MR_VERSION_MAJOR}.${MR_VERSION_MINOR}.${MR_VERSION_PATCH}
	LANGUAGES C CXX)

add_executable(mrc main.c)
