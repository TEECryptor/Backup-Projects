
TestCOMps.dll: dlldata.obj TestCOM_p.obj TestCOM_i.obj
	link /dll /out:TestCOMps.dll /def:TestCOMps.def /entry:DllMain dlldata.obj TestCOM_p.obj TestCOM_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del TestCOMps.dll
	@del TestCOMps.lib
	@del TestCOMps.exp
	@del dlldata.obj
	@del TestCOM_p.obj
	@del TestCOM_i.obj
