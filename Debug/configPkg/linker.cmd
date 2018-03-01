/*
 * Do not modify this file; it is automatically generated from the template
 * linkcmd.xdt in the ti.targets.elf package and will be overwritten.
 */

/*
 * put '"'s around paths because, without this, the linker
 * considers '-' as minus operator, not a file name character.
 */


-l"C:\Users\dong.x\workspace_v6_2\Test\Debug\configPkg\package\cfg\app_pe9.oe9"
-l"C:\ti\uia_2_00_06_52\packages\ti\uia\sysbios\lib\release\ti.uia.sysbios.ae9"
-l"C:\ti\uia_2_00_06_52\packages\ti\uia\loggers\lib\release\ti.uia.loggers.ae9"
-l"C:\Users\dong.x\workspace_v6_2\Test\src\sysbios\sysbios.ae9"
-l"C:\ti\uia_2_00_06_52\packages\ti\uia\services\lib\release\ti.uia.services.ae9"
-l"C:\ti\uia_2_00_06_52\packages\ti\uia\runtime\lib\release\ti.uia.runtime.ae9"
-l"C:\ti\uia_2_00_06_52\packages\ti\uia\events\lib\release\ti.uia.events.ae9"
-l"C:\ti\bios_6_46_05_55\packages\ti\targets\arm\rtsarm\lib\ti.targets.arm.rtsarm.ae9"
-l"C:\ti\bios_6_46_05_55\packages\ti\targets\arm\rtsarm\lib\boot.ae9"
-l"C:\ti\bios_6_46_05_55\packages\ti\targets\arm\rtsarm\lib\auto_init.ae9"

--retain="*(xdc.meta)"

/* C6x Elf symbols */
--symbol_map __TI_STACK_SIZE=__STACK_SIZE
--symbol_map __TI_STACK_BASE=__stack
--symbol_map _stack=__stack



--args 0x0
-heap  0x0
-stack 0x800

MEMORY
{
    IRAM (RWX) : org = 0xffff0000, len = 0x2000
    L3_CBA_RAM (RWX) : org = 0x80000000, len = 0x20000
    DDR : org = 0xc3000000, len = 0x1000000
}

/*
 * Linker command file contributions from all loaded packages:
 */

/* Content from xdc.services.global (null): */

/* Content from xdc (null): */

/* Content from xdc.corevers (null): */

/* Content from xdc.shelf (null): */

/* Content from xdc.services.spec (null): */

/* Content from xdc.services.intern.xsr (null): */

/* Content from xdc.services.intern.gen (null): */

/* Content from xdc.services.intern.cmd (null): */

/* Content from xdc.bld (null): */

/* Content from ti.targets (null): */

/* Content from ti.targets.arm.elf (null): */

/* Content from xdc.rov (null): */

/* Content from xdc.runtime (null): */

/* Content from ti.targets.arm.rtsarm (null): */

/* Content from ti.sysbios.interfaces (null): */

/* Content from ti.sysbios.family (null): */

/* Content from ti.sysbios.family.arm (ti/sysbios/family/arm/linkcmd.xdt): */
--retain "*(.vecs)"

/* Content from xdc.services.getset (null): */

/* Content from xdc.rta (null): */

/* Content from ti.uia.events (null): */

/* Content from ti.uia.runtime (null): */

/* Content from xdc.runtime.knl (null): */

/* Content from ti.sysbios.family.arm.arm9 (null): */

/* Content from ti.sysbios.rts (ti/sysbios/rts/linkcmd.xdt): */

/* Content from ti.uia.services (null): */

/* Content from ti.catalog.arm (null): */

/* Content from ti.catalog.arm.peripherals.timers (null): */

/* Content from ti.catalog (null): */

/* Content from xdc.platform (null): */

/* Content from xdc.cfg (null): */

/* Content from ti.catalog.peripherals.hdvicp2 (null): */

/* Content from ti.catalog.c6000 (null): */

/* Content from ti.platforms.evmOMAPL138 (null): */

/* Content from ti.sysbios.hal (null): */

/* Content from ti.sysbios.knl (null): */

/* Content from ti.sysbios.timers.timer64 (null): */

/* Content from ti.sysbios (null): */

/* Content from ti.sysbios.syncs (null): */

/* Content from ti.sysbios.family.arm.exc (null): */

/* Content from ti.sysbios.family.arm.da830 (ti/sysbios/family/arm/da830/linkcmd.xdt): */
--retain "*(.vectorTable)"

/* Content from ti.sysbios.gates (null): */

/* Content from ti.sysbios.xdcruntime (null): */

/* Content from ti.sysbios.heaps (null): */

/* Content from ti.uia.loggers (null): */

/* Content from ti.sysbios.utils (null): */

/* Content from ti.uia.sysbios (null): */

/* Content from configPkg (null): */

/* Content from xdc.services.io (null): */


/*
 * symbolic aliases for static instance objects
 */
xdc_runtime_Startup__EXECFXN__C = 1;
xdc_runtime_Startup__RESETFXN__C = 1;

SECTIONS
{
    .text: load >> DDR
    .stack: load > DDR
    GROUP: load > DDR
    {
        .bss:
        .neardata:
        .rodata:
    }
    .binit: load > DDR
    .cinit: load > DDR
    .init_array: load > DDR
    .const: load >> DDR
    .data: load >> DDR
    .fardata: load >> DDR
    .switch: load >> DDR
    .sysmem: load > DDR
    .far: load >> DDR
    .args: load > DDR align = 0x4, fill = 0 {_argsize = 0x0; }
    .cio: load >> DDR
    .ARM.exidx: load > DDR
    .ARM.extab: load >> DDR
    .vecs: load > 0xffff0000
    .vectorTable: load > 0xffff0040
    ti.sysbios.family.arm.arm9.mmuTableSection: load > DDR, type = NOINIT
    xdc.meta: load > DDR, type = COPY

}
