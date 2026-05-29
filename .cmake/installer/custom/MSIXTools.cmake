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
    set(CPACK_MSIX_APPLICATIONS "${CPACK_MSIX_APPLICATIONS}]}")
endmacro()

# 1. Define your data as a literal JSON string
set(MANIFEST_APPS_JSON "
{
    \"applications\": [
        {
            \"id\": \"JuggernyautCompiler\",
            \"name\": \"Juggernyaut Compiler\",
            \"executable\": \"bin\\\\jug-cmp.exe\",
            \"aliases\": [\"jug-cmp.exe\", \"jc.exe\"]
        },
        {
            \"id\": \"JuggernyautTool\",
            \"name\": \"Juggernyaut Utility\",
            \"executable\": \"bin\\\\jug-tool.exe\",
            \"aliases\": [\"jug-tool.exe\"]
        }
    ]
}
")

# 2. Get the number of applications in the array
string(JSON APP_COUNT LENGTH "${MANIFEST_APPS_JSON}" "applications")
math(EXPR APP_LAST_INDEX "${APP_COUNT} - 1")

# 3. Loop through the JSON array
foreach(INDEX RANGE ${APP_LAST_INDEX})
    # Extract the scalar values
    string(JSON CURRENT_ID GET "${MANIFEST_APPS_JSON}" "applications" ${INDEX} "id")
    string(JSON CURRENT_NAME GET "${MANIFEST_APPS_JSON}" "applications" ${INDEX} "name")
    string(JSON CURRENT_EXEC GET "${MANIFEST_APPS_JSON}" "applications" ${INDEX} "executable")

    message(STATUS "Processing App: ${CURRENT_NAME} (${CURRENT_ID}) -> ${CURRENT_EXEC}")

    # Loop through the nested aliases array
    string(JSON ALIAS_COUNT LENGTH "${MANIFEST_APPS_JSON}" "applications" ${INDEX} "aliases")
    math(EXPR ALIAS_LAST_INDEX "${ALIAS_COUNT} - 1")
    
    foreach(ALIAS_INDEX RANGE ${ALIAS_LAST_INDEX})
        string(JSON CURRENT_ALIAS GET "${MANIFEST_APPS_JSON}" "applications" ${INDEX} "aliases" ${ALIAS_INDEX})
        message(STATUS "  - Found Alias: ${CURRENT_ALIAS}")
    endforeach()
endforeach()

