set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB RECEIVER_LOGIC_SRC *.h *.cpp)

add_library(receiver_logic STATIC
    ${RECEIVER_LOGIC_SRC}
)
set_target_properties(receiver_logic PROPERTIES POSITION_INDEPENDENT_CODE ON)


target_link_libraries(receiver_logic PUBLIC
    Qt5::Core
    Qt5::SerialPort
)

set_target_properties(receiver_logic PROPERTIES CXX_STANDARD 17)