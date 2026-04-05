set(SRC
    main.cpp
    ui.cpp
)

idf_component_register(
    SRCS ${SRC}
    INCLUDE_DIRS "."
    REQUIRES driver esp_lcd
)

target_compile_definitions(${COMPONENT_LIB} PUBLIC
    LV_CONF_INCLUDE_SIMPLE
)
