#
_XDCBUILDCOUNT = 
ifneq (,$(findstring path,$(_USEXDCENV_)))
override XDCPATH = C:/ti/bios_6_46_05_55/packages;C:/ti/pdk_omapl138_1_0_2/packages;C:/ti/uia_2_00_06_52/packages;C:/ti/ccs620/ccsv6/ccs_base;C:/Users/dong.x/workspace_v6_2/Test/.config
override XDCROOT = C:/ti/ccs620/xdctools_3_32_01_22_core
override XDCBUILDCFG = ./config.bld
endif
ifneq (,$(findstring args,$(_USEXDCENV_)))
override XDCARGS = 
override XDCTARGETS = 
endif
#
ifeq (0,1)
PKGPATH = C:/ti/bios_6_46_05_55/packages;C:/ti/pdk_omapl138_1_0_2/packages;C:/ti/uia_2_00_06_52/packages;C:/ti/ccs620/ccsv6/ccs_base;C:/Users/dong.x/workspace_v6_2/Test/.config;C:/ti/ccs620/xdctools_3_32_01_22_core/packages;..
HOSTOS = Windows
endif
