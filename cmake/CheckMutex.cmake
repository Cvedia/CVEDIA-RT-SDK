include(CheckCXXSymbolExists)

# Save the original CMAKE_CXX_FLAGS to a temporary variable
string(APPEND CMAKE_CXX_FLAGS_T "${CMAKE_CXX_FLAGS}")

# Add the desired compile options
if(NOT WIN32 OR MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif()

# Remove -Werror from CMAKE_CXX_FLAGS for the symbol check
string(REPLACE "-Werror" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR MSVC)
    set(HAVE_SHARED_MUTEX 1)
else()
    check_cxx_symbol_exists("std::shared_mutex()" shared_mutex HAVE_SHARED_MUTEX)
    check_cxx_symbol_exists("std::shared_timed_mutex()" shared_mutex HAVE_SHARED_TIMED_MUTEX)
endif()

# Restore the original CMAKE_CXX_FLAGS value
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_T}")
# Clear the temporary variable
unset(CMAKE_CXX_FLAGS_T)

if (NOT HAVE_SHARED_MUTEX AND NOT HAVE_SHARED_TIMED_MUTEX)
    message(FATAL_ERROR "No shared mutex implementation found")
endif()
