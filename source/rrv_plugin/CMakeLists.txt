set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB RRV_PLUGIN_SRC *.h *.cpp *.ui)

add_library(plugin SHARED
    ${RRV_PLUGIN_SRC}
    )

target_link_libraries(rrv_plugin PUBLIC
    SkydelPlugin
    )

set_target_properties(plugin PROPERTIES CXX_STANDARD 17)

target_compile_definitions(plugin PRIVATE PLUGIN_IID="rrv_plugin"
                                          PLUGIN_FILE="rrv_plugin.json")

install(TARGETS plugin RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR})
