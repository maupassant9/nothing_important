#
#  Do not edit this file.  This file is generated from 
#  package.bld.  Any modifications to this file will be 
#  overwritten whenever makefiles are re-generated.
#
#  target compatibility key = ti.targets.arm.elf.Arm9{1,0,15.12,3
#
ifeq (,$(MK_NOGENDEPS))
-include package/cfg/app_pe9.oe9.dep
package/cfg/app_pe9.oe9.dep: ;
endif

package/cfg/app_pe9.oe9: | .interfaces
package/cfg/app_pe9.oe9: package/cfg/app_pe9.c package/cfg/app_pe9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/armcl -c  -g --optimize_with_debug -qq -pdsw225 -ms -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_pe9.h"'  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_15_12_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s oe9 $< -C   -g --optimize_with_debug -qq -pdsw225 -ms -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_pe9.h"'  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_15_12_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/app_pe9.oe9: export C_DIR=
package/cfg/app_pe9.oe9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/cfg/app_pe9.oe9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

package/cfg/app_pe9.se9: | .interfaces
package/cfg/app_pe9.se9: package/cfg/app_pe9.c package/cfg/app_pe9.mak
	@$(RM) $@.dep
	$(RM) $@
	@$(MSG) cle9 -n $< ...
	$(ti.targets.arm.elf.Arm9.rootDir)/bin/armcl -c -n -s --symdebug:none -g --optimize_with_debug -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_pe9.h"'  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_15_12_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/cfg -fr=./package/cfg -fc $<
	$(MKDEP) -a $@.dep -p package/cfg -s oe9 $< -C  -n -s --symdebug:none -g --optimize_with_debug -qq -pdsw225 -me -mv5e --abi=eabi -eo.oe9 -ea.se9   -Dxdc_cfg__xheader__='"xconfig_app/package/cfg/app_pe9.h"'  -Dxdc_target_name__=Arm9 -Dxdc_target_types__=ti/targets/arm/elf/std.h -Dxdc_bld__profile_release -Dxdc_bld__vers_1_0_15_12_3 -O2  $(XDCINCS) -I$(ti.targets.arm.elf.Arm9.rootDir)/include  -fs=./package/cfg -fr=./package/cfg
	-@$(FIXDEP) $@.dep $@.dep
	
package/cfg/app_pe9.se9: export C_DIR=
package/cfg/app_pe9.se9: PATH:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)
package/cfg/app_pe9.se9: Path:=$(ti.targets.arm.elf.Arm9.rootDir)/bin/;$(PATH)

clean,e9 ::
	-$(RM) package/cfg/app_pe9.oe9
	-$(RM) package/cfg/app_pe9.se9

app.pe9: package/cfg/app_pe9.oe9 package/cfg/app_pe9.mak

clean::
	-$(RM) package/cfg/app_pe9.mak
