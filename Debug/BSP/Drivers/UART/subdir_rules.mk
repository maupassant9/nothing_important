################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
BSP/Drivers/UART/uart.obj: ../BSP/Drivers/UART/uart.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv5e --code_state=32 -me --include_path="C:/ti/OMAPL138_StarterWare_1_10_04_01" --include_path="C:/ti/OMAPL138_StarterWare_1_10_04_01/include/hw" --include_path="C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include" -g --define=omapl138 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="BSP/Drivers/UART/uart.d" --obj_directory="BSP/Drivers/UART" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


