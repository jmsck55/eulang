
include std/dll.e

sequence dllname

ifdef WINDOWS then
dllname = "libeulang.dll"
elsifdef OSX then
dllname = "libeulang.dylib"
elsedef
dllname = "libeulang.so"
end ifdef
atom mydll = open_dll(dllname)
if mydll = 0 then
	printf(1, "Error: Couldn't open %s\n", {dllname})
	abort(1)
end if

integer version_id = define_c_func(mydll, "version", {}, C_INT)

? c_func(version_id, {})

