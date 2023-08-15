#Find the source file in the src folder recursively
file(GLOB_RECURSE SOURCES src/*.c)
#Set the include directory
set(included_dirs inc)
#Register to ESP32
idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS ${included_dirs}
                       )