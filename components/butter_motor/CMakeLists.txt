if(DEFINED ENV{BLUEPAD32_ARDUINO})
    set(srcs "butterMotor.cpp")
    set(requires "bluepad32_arduino" "arduino")
    idf_component_register(SRCS ${srcs}
                        INCLUDE_DIRS "include"
                        REQUIRES ${requires})
endif()
