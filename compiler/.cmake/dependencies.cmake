message(STATUS "[DEPENDENCIES] Checking dependencies...")

include(FetchContent)

# VERSION CONTROL
# Manage the versions for used dependencies

# {fmt} library
set(FMT_LIB_VERSION 10.2.1)
#Currently, I can't figure out why v11.1.4 doesn't like c++20
