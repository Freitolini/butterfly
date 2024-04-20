if(DEFINED ENV{BLUEPAD32_ARDUINO})
    set(srcs "Adafruit_NeoPixel.cpp" "esp.c")
    set(requires "bluepad32_arduino" "arduino")
    idf_component_register(SRCS ${srcs}
                        INCLUDE_DIRS "include"
                        REQUIRES ${requires})
endif()
