# Target OS
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Cross-compiler
set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER  x86_64-w64-mingw32-windres)

# Cross-compiler sysroot
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Only search the sysroot for libraries/headers
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Static linking to avoid DLL dependencies
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
