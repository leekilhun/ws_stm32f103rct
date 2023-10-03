cmake_minimum_required(VERSION 3.12)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/tools")
include(arm-none-eabi-gcc)

set(PRJ_NAME stm32f103rct_ResetCnt_boot)

project(${PRJ_NAME}
  LANGUAGES ASM C CXX
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE ${PRJ_NAME}.elf)
set(LINKER_FILE ${CMAKE_SOURCE_DIR}/STM32F103RCTX_FLASH.ld)


file(GLOB SRC_FILES CONFIGURE_DEPENDS
  App/hw/*.c
  App/hw/driver/*.c
  App/hw/driver/usb/*.c
  App/hw/driver/usb/usb_cdc/*.c
  App/exhw/*.c
  App/exhw/driver/*.c
  App/bsp/*.c

  Core/Src/main.cpp
  Core/Src/stm32f1xx_hal_msp.c
  Core/Src/stm32f1xx_it.c
  Core/Src/syscalls.c
  Core/Src/sysmem.c
  Core/Src/system_stm32f1xx.c
)

file(GLOB_RECURSE SRC_FILES_RECURSE CONFIGURE_DEPENDS
App/*.cpp
App/ap/*.cpp
App/ap/*.c
App/ap/boot/*.c
App/common/*.c    
Drivers/STM32F1xx_HAL_Driver/Src/*.c

lib/STM32_USB_Device_Library/Core/Src/*.c
  #lib/rtos/*.c
)

add_executable(${EXECUTABLE} 
  Core/Src/main.cpp
  Core/Startup/startup_stm32f103rctx.s

  ${SRC_FILES}
  ${SRC_FILES_RECURSE}
)

target_compile_definitions(${EXECUTABLE} PRIVATE
  -DSTM32F103xE
  #-D_USE_HW_RTOS
  #-DUSER_VECT_TAB_ADDRESS
  -DDEBUG
  )

target_include_directories(${EXECUTABLE} PRIVATE 
  App  
  App/ap  
  App/ap/boot
  App/bsp
  App/common 
  App/common/core
  App/common/inc_hw
  #App/common/inc_exhw
  #App/exhw
  App/hw/driver/usb
  App/hw/driver/usb/usb_cdc
  App/hw
  Core/Inc
  
  Drivers/CMSIS/Device/ST/STM32F1xx/Include
  Drivers/CMSIS/Include
  Drivers/STM32F1xx_HAL_Driver/Inc

 lib/STM32_USB_Device_Library/Core/Inc

  #lib/rtos
  #lib/rtos/Source/CMSIS_RTOS
  #lib/rtos/Source/portable/GCC/ARM_CM0
  #lib/rtos/Source/include
)



target_compile_options(${EXECUTABLE} PRIVATE
  -mcpu=cortex-m3
  -mthumb

  #-mfpu=fpv4-sp-d16
  #-mfloat-abi=soft

  -fdata-sections
  -ffunction-sections

  -Wall
  -Os
  -g3
  )


target_link_options(${EXECUTABLE} PRIVATE
-T${LINKER_FILE}
-mcpu=cortex-m3
-mthumb
#-mfpu=fpv4-sp-d16
#-mfloat-abi=soft
-specs=nano.specs
-lc
-lm
-Wl,-Map=${PROJECT_NAME}.map,--cref
-Wl,--gc-sections
-Xlinker -print-memory-usage -Xlinker
)


add_custom_command(TARGET ${EXECUTABLE} 
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin
    COMMENT "Invoking: Make Binary"
    )

#set_target_properties(${EXECUTABLE} PROPERTIES SUFFIX ".elf")
#add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
#        COMMAND arm-none-eabi-size --format=berkeley --totals "$<TARGET_FILE:${EXECUTABLE}>"
#        COMMENT "Invoking: Cross ARM GNU Print Size")