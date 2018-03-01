################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-1704950689: ../app.cfg
	@echo 'Building file: $<'
	@echo 'Invoking: XDCtools'
	"C:/ti/ccs620/xdctools_3_32_01_22_core/xs" --xdcpath="C:/ti/bios_6_46_05_55/packages;C:/ti/pdk_omapl138_1_0_2/packages;C:/ti/uia_2_00_06_52/packages;C:/ti/ccs620/ccsv6/ccs_base;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.Arm9 -p ti.platforms.evmOMAPL138 -r release -c "C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS" --compileOptions "-g --optimize_with_debug" "$<"
	@echo 'Finished building: $<'
	@echo ' '

configPkg/linker.cmd: build-1704950689
configPkg/compiler.opt: build-1704950689
configPkg/: build-1704950689

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl" -mv5e --code_state=32 -me --include_path="C:/ti/OMAPL138_StarterWare_1_10_04_01" --include_path="C:/ti/OMAPL138_StarterWare_1_10_04_01/include/hw" --include_path="C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include" -g --define=omapl138 --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


