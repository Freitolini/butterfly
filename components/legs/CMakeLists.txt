if(DEFINED ENV{BLUEPAD32_ARDUINO})
    set(srcs "legManager.cpp" "leg.cpp")
    set(requires "bluepad32_arduino" "arduino" "leds")
    idf_component_register(SRCS ${srcs}
                        INCLUDE_DIRS "include"
                        REQUIRES ${requires})
endif()
