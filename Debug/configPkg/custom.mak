## THIS IS A GENERATED FILE -- DO NOT EDIT
.configuro: .libraries,e9 linker.cmd package/cfg/app_pe9.oe9

# To simplify configuro usage in makefiles:
#     o create a generic linker command file name 
#     o set modification times of compiler.opt* files to be greater than
#       or equal to the generated config header
#
linker.cmd: package/cfg/app_pe9.xdl
	$(SED) 's"^\"\(package/cfg/app_pe9cfg.cmd\)\"$""\"C:/Users/dong.x/workspace_v6_2/Test/Debug/configPkg/\1\""' package/cfg/app_pe9.xdl > $@
	-$(SETDATE) -r:max package/cfg/app_pe9.h compiler.opt compiler.opt.defs
