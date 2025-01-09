set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB RRV_PLUGIN_SRC *.h *.cpp *.ui)

add_library(rrv_plugin SHARED
    ${RRV_PLUGIN_SRC}
    )

target_link_libraries(rrv_plugin PUBLIC
    SkydelPlugin
    Qt5::Core
    Qt5::SerialPort
    receiver_logic
    )

set_target_properties(rrv_plugin PROPERTIES CXX_STANDARD 17)

target_compile_definitions(rrv_plugin PRIVATE PLUGIN_IID="rrv_plugin"
                                          PLUGIN_FILE="rrv_plugin.json")

install(TARGETS rrv_plugin RUNTIME DESTINATION ${PLUGIN_INSTALL_DIR})
