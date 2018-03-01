
XOPTS = -I"C:/ti/ccs620/xdctools_3_32_01_22_core/packages/" -Dxdc_target_types__=C:/ti/bios_6_46_05_55/packages/ti/targets/arm/elf/std.h -Dxdc_target_name__=Arm9

vpath % C:/ti/bios_6_46_05_55/packages/ti/sysbios/
vpath %.c C:/ti/ccs620/xdctools_3_32_01_22_core/packages/

CCOPTS    = -me -mv5e --abi=eabi -q -ms --opt_for_speed=2  --program_level_compile -o3 -g --optimize_with_debug  -Dti_sysbios_knl_Task_minimizeLatency__D=FALSE -Dti_sysbios_family_arm_exc_Exception_enableDecode__D=TRUE -Dti_sysbios_knl_Clock_stopCheckNext__D=FALSE -Dti_sysbios_hal_Core_numCores__D=1

XDC_ROOT  = C:/ti/ccs620/xdctools_3_32_01_22_core/packages/

BIOS_ROOT = C:/ti/bios_6_46_05_55/packages/ti/sysbios/

BIOS_DEFS =  -Dti_sysbios_BIOS_swiEnabled__D=TRUE -Dti_sysbios_BIOS_taskEnabled__D=TRUE -Dti_sysbios_BIOS_clockEnabled__D=TRUE -Dti_sysbios_BIOS_runtimeCreatesEnabled__D=TRUE -Dti_sysbios_knl_Task_moduleStateCheckFlag__D=FALSE -Dti_sysbios_knl_Task_objectCheckFlag__D=FALSE -Dti_sysbios_hal_Hwi_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Swi_DISABLE_ALL_HOOKS -Dti_sysbios_BIOS_smpEnabled__D=FALSE -Dti_sysbios_Build_useHwiMacros -Dti_sysbios_knl_Swi_numPriorities__D=16 -Dti_sysbios_knl_Task_deleteTerminatedTasks__D=FALSE -Dti_sysbios_knl_Task_numPriorities__D=32 -Dti_sysbios_knl_Task_checkStackFlag__D=TRUE -Dti_sysbios_knl_Task_initStackFlag__D=TRUE -Dti_sysbios_knl_Task_DISABLE_ALL_HOOKS -Dti_sysbios_knl_Clock_TICK_SOURCE=ti_sysbios_knl_Clock_TickSource_TIMER -Dti_sysbios_knl_Clock_TICK_MODE=ti_sysbios_knl_Clock_TickMode_PERIODIC -Dti_sysbios_hal_Core_delegate_getId=ti_sysbios_hal_CoreNull_getId__E -Dti_sysbios_hal_Core_delegate_interruptCore=ti_sysbios_hal_CoreNull_interruptCore__E -Dti_sysbios_hal_Core_delegate_lock=ti_sysbios_hal_CoreNull_lock__E -Dti_sysbios_hal_Core_delegate_unlock=ti_sysbios_hal_CoreNull_unlock__E -Dti_sysbios_hal_Core_numCores__D=1 -Dti_sysbios_hal_CoreNull_numCores__D=1 -Dti_sysbios_utils_Load_taskEnabled__D=FALSE -Dti_sysbios_utils_Load_swiEnabled__D=FALSE -Dti_sysbios_utils_Load_hwiEnabled__D=FALSE -Dti_sysbios_family_arm_da830_Hwi_dispatcherSwiSupport__D=TRUE -Dti_sysbios_family_arm_da830_Hwi_dispatcherTaskSupport__D=TRUE -Dti_sysbios_family_arm_da830_Hwi_dispatcherAutoNestingSupport__D=TRUE -Dti_sysbios_family_arm_da830_Hwi_dispatcherIrpTrackingSupport__D=TRUE -Dti_sysbios_knl_Semaphore_supportsEvents__D=FALSE -Dti_sysbios_knl_Semaphore_supportsPriority__D=TRUE 

BIOS_INC  = -I"C:/ti/bios_6_46_05_55/packages/"

INCS =  $(BIOS_INC) -I"C:/ti/bios_6_46_05_55/packages/" -I"C:/ti/pdk_omapl138_1_0_2/packages/" -I"C:/ti/uia_2_00_06_52/packages/" -I"C:/ti/ccs620/ccsv6/ccs_base/" -I"C:/ti/ccs620/xdctools_3_32_01_22_core/packages/" -I"../" -I"/packages//" 

CC = C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl -c $(CCOPTS) -I C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include
ASM = C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armcl -c $(CCOPTS) -I C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/include
AR = C:/ti/ccs620/ccsv6/tools/compiler/arm_15.12.3.LTS/bin/armar rq

DEL = C:/ti/ccs620/xdctools_3_32_01_22_core/packages/../bin/rm -f
CP = C:/ti/ccs620/xdctools_3_32_01_22_core/packages/../bin/cp -f

define RM
	$(if $(wildcard $1),$(DEL) $1,:)
endef

define ASSEMBLE
	@echo asme9 $< ...
	@$(ASM) $(BIOS_DEFS) $(XOPTS) $(INCS) $<
endef

all: sysbios.ae9

arm_HwiCommon_asm.obj: family/arm/HwiCommon_asm.asm makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=arm_HwiCommon_asm.obj

arm_IntrinsicsSupport_asm.obj: family/arm/IntrinsicsSupport_asm.asm makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=arm_IntrinsicsSupport_asm.obj

arm_TaskSupport_asm.obj: family/arm/TaskSupport_asm.asm makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=arm_TaskSupport_asm.obj

arm9_Cache_asm.obj: family/arm/arm9/Cache_asm.asm makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=arm9_Cache_asm.obj

arm9_Mmu_asm.obj: family/arm/arm9/Mmu_asm.asm makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=arm9_Mmu_asm.obj

da830_Hwi_asm.obj: family/arm/da830/Hwi_asm.s470 makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=da830_Hwi_asm.obj

exc_Exception_asm.obj: family/arm/exc/Exception_asm.asm makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=exc_Exception_asm.obj

timer64_Timer_asm.obj: timers/timer64/Timer_asm.s470 makefile
	@-$(call RM, $@)
	$(ASSEMBLE) --output_file=timer64_Timer_asm.obj


BIOS.obj: BIOS.c family/arm/HwiCommon.c family/arm/IntrinsicsSupport.c family/arm/TaskSupport.c knl/Clock.c knl/Idle.c knl/Intrinsics.c knl/Event.c knl/Mailbox.c knl/Queue.c knl/Semaphore.c knl/Swi.c knl/Swi_andn.c knl/Task.c family/arm/arm9/Cache.c family/arm/arm9/Mmu.c hal/Cache.c hal/Core.c hal/CoreNull.c hal/Hwi.c hal/Hwi_stack.c hal/Hwi_startup.c hal/Timer.c family/arm/da830/Hwi.c family/arm/da830/TimerSupport.c family/arm/exc/Exception.c gates/GateHwi.c gates/GateMutex.c heaps/HeapMem.c heaps/HeapTrack.c timers/timer64/Timer.c timers/timer64/TimestampProvider.c utils/Load.c utils/Load_CPU.c makefile
	@-$(call RM, $@)
	@echo cle9 $< ...
	@$(CC) $(BIOS_DEFS) $(XOPTS) $(INCS) \
	$(BIOS_ROOT)BIOS.c \
	$(BIOS_ROOT)family/arm/HwiCommon.c \
	$(BIOS_ROOT)family/arm/IntrinsicsSupport.c \
	$(BIOS_ROOT)family/arm/TaskSupport.c \
	$(BIOS_ROOT)knl/Clock.c \
	$(BIOS_ROOT)knl/Idle.c \
	$(BIOS_ROOT)knl/Intrinsics.c \
	$(BIOS_ROOT)knl/Event.c \
	$(BIOS_ROOT)knl/Mailbox.c \
	$(BIOS_ROOT)knl/Queue.c \
	$(BIOS_ROOT)knl/Semaphore.c \
	$(BIOS_ROOT)knl/Swi.c \
	$(BIOS_ROOT)knl/Swi_andn.c \
	$(BIOS_ROOT)knl/Task.c \
	$(BIOS_ROOT)family/arm/arm9/Cache.c \
	$(BIOS_ROOT)family/arm/arm9/Mmu.c \
	$(BIOS_ROOT)hal/Cache.c \
	$(BIOS_ROOT)hal/Core.c \
	$(BIOS_ROOT)hal/CoreNull.c \
	$(BIOS_ROOT)hal/Hwi.c \
	$(BIOS_ROOT)hal/Hwi_stack.c \
	$(BIOS_ROOT)hal/Hwi_startup.c \
	$(BIOS_ROOT)hal/Timer.c \
	$(BIOS_ROOT)family/arm/da830/Hwi.c \
	$(BIOS_ROOT)family/arm/da830/TimerSupport.c \
	$(BIOS_ROOT)family/arm/exc/Exception.c \
	$(BIOS_ROOT)gates/GateHwi.c \
	$(BIOS_ROOT)gates/GateMutex.c \
	$(BIOS_ROOT)heaps/HeapMem.c \
	$(BIOS_ROOT)heaps/HeapTrack.c \
	$(BIOS_ROOT)timers/timer64/Timer.c \
	$(BIOS_ROOT)timers/timer64/TimestampProvider.c \
	$(BIOS_ROOT)utils/Load.c \
	$(BIOS_ROOT)utils/Load_CPU.c \

sysbios.ae9: BIOS.obj arm_HwiCommon_asm.obj arm_IntrinsicsSupport_asm.obj arm_TaskSupport_asm.obj arm9_Cache_asm.obj arm9_Mmu_asm.obj da830_Hwi_asm.obj exc_Exception_asm.obj timer64_Timer_asm.obj
	@-$(call RM, $@)
	@echo are9 $^ ...
	@$(AR) $@ $^


clean:
	@$(DEL) ..\makefile.libs
	@-$(call RM, *)
