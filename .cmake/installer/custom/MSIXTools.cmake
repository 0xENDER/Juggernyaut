cmake_minimum_required(VERSION 4.2 FATAL_ERROR)

####################################################
## USE
####################################################

# include(".../MSIXTools.cmake")

####################################################
## Application alias definition
## INPUT:
##      - TARGET (The name of the executable in your runtime directory)
##      - DISPLAY NAME (The name shown in system UIs)
##      - DESCRIPTION
##      - ALIASES (accessed through the command line)
####################################################
macro(cpack_msix_add_application_alias TARGET DISPLAY_NAME DESCRIPTION)
    if(CPACK_MSIX_APPLICATIONS STREQUAL "")
        set(CPACK_MSIX_APPLICATIONS "{\n")
    else()
        set(CPACK_MSIX_APPLICATIONS "${CPACK_MSIX_APPLICATIONS},{\n")
    endif()
    set(CPACK_MSIX_APPLICATIONS "${CPACK_MSIX_APPLICATIONS}
    \"type\": \"ALIAS\",
    \"target\": \"${TARGET}\",
    \"name\": \"${DISPLAY_NAME}\",
    \"description\": \"${DESCRIPTION}\",
    \"aliases\": [")
    set(ALIAS_LIST "")
    foreach(ALIAS IN LISTS ARGN)
        if(ALIAS_LIST STREQUAL "")
            set(ALIAS_LIST "\"${ALIAS}\"")
        else()
            set(ALIAS_LIST "${ALIAS_LIST}, \"${ALIAS}\"")
        endif()
    endforeach()
    set(CPACK_MSIX_APPLICATIONS "${CPACK_MSIX_APPLICATIONS}${ALIAS_LIST}]}")
endmacro()
