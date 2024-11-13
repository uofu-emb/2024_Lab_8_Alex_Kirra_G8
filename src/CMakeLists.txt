# This is the main binary. List your C files here.
add_executable(can_send
    can_send.c
    )

pico_set_program_name(can_send "test")
pico_set_program_version(can_send "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can_send 0)
pico_enable_stdio_usb(can_send 1)

# This is the main binary. List your C files here.
add_executable(can_receive
    can_receive.c
    )

pico_set_program_name(can_receive "test")
pico_set_program_version(can_receive "0.1")

# Modify the below lines to enable/disable output over UART/USB.
# These are mutually exclusive, only set one.
pico_enable_stdio_uart(can_receive 0)
pico_enable_stdio_usb(can_receive 1)



# Add the libraries to the build
target_link_libraries(can_send PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

target_link_libraries(can_receive PRIVATE
  pico_stdlib
  pico_multicore
  pico_async_context_freertos
  FreeRTOS-Kernel-Heap4
  can2040
)

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(can_send PRIVATE
        pico_cyw43_arch_none
    )
endif()

# The CYW43 is the Wifi/Bluetooth module. If the board is set to pico_w, this
# variable will be set and the wireless libraries added.
if(PICO_CYW43_SUPPORTED)
    # For led support on pico_w
    target_link_libraries(can_receive PRIVATE
        pico_cyw43_arch_none
    )
endif()

# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can_send)
# Another pico macro which generates the uf2 file in addition to the elf
pico_add_extra_outputs(can_receive)

add_custom_target(flash_send picotool load -f $<TARGET_FILE:can_send> DEPENDS can_send)
add_custom_target(flash_receive picotool load -f $<TARGET_FILE:can_receive> DEPENDS can_receive)
