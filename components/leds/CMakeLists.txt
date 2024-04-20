if(DEFINED ENV{BLUEPAD32_ARDUINO})
    set(srcs "leds.cpp")
    set(requires "bluepad32_arduino" "arduino" "neopixel")
    idf_component_register(SRCS ${srcs}
                        INCLUDE_DIRS "include"
                        REQUIRES ${requires})
endif()
