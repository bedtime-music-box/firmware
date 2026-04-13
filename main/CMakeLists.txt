set(SRC
    img/splash.c
    fs.cpp
    main.cpp
    screens/boot.cpp
    ui.cpp
    util/lvgllocker.cpp
)

idf_component_register(
    SRCS ${SRC}
    INCLUDE_DIRS "."
    REQUIRES fatfs sdmmc driver esp_lcd vfs
)

target_compile_definitions(${COMPONENT_LIB} PUBLIC
    LV_CONF_INCLUDE_SIMPLE
)
