# invoke SourceDir generated makefile for app.pe9
app.pe9: .libraries,app.pe9
.libraries,app.pe9: package/cfg/app_pe9.xdl
	$(MAKE) -f C:\Users\dong.x\workspace_v6_2\Test/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\dong.x\workspace_v6_2\Test/src/makefile.libs clean

