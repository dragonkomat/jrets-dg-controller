add_executable(jrets-dg-controller
        main.c
        button.c
        footcon.c
        mascon.c
        reverser.c
        usb_main.c
        usb_descriptors.c
        )

# pull in common dependencies
target_link_libraries(jrets-dg-controller pico_stdlib hardware_adc pico_multicore tinyusb_device)

include_directories(jrets-dg-controller PRIVATE ${CMAKE_CURRENT_LIST_DIR})

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(jrets-dg-controller)
