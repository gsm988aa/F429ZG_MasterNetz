################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ioLibrary_Driver/Internet/DNS/dns.c 

OBJS += \
./Core/ioLibrary_Driver/Internet/DNS/dns.o 

C_DEPS += \
./Core/ioLibrary_Driver/Internet/DNS/dns.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ioLibrary_Driver/Internet/DNS/dns.o: ../Core/ioLibrary_Driver/Internet/DNS/dns.c Core/ioLibrary_Driver/Internet/DNS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I"C:/Users/Administrator/Desktop/F429ZG_MasterNetz/Core/ioLibrary_Driver/Application/loopback" -I"C:/Users/Administrator/Desktop/F429ZG_MasterNetz/Core/ioLibrary_Driver/Ethernet" -I"C:/Users/Administrator/Desktop/F429ZG_MasterNetz/Core/ioLibrary_Driver/Ethernet/W5500" -IC:/Users/Administrator/STM32Cube/Repository/STM32Cube_FW_F4_V1.26.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/Administrator/STM32Cube/Repository/STM32Cube_FW_F4_V1.26.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/Administrator/STM32Cube/Repository/STM32Cube_FW_F4_V1.26.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/Administrator/STM32Cube/Repository/STM32Cube_FW_F4_V1.26.1/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/ioLibrary_Driver/Internet/DNS/dns.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

