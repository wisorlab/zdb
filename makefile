cc = gcc
cflags = -std=c11 -m64 -Wall
sqlite = sqlite3
sqlite_ext = .dll
sqlite_flags = -shared -lpthread -ldl
sqlite_dir = lib/
src_dir = src/

# bld_dir = +build/
bld_dir = 

mex_flags = -DMATLAB_MEX_FILE -shared -Wl,--export-all-symbols
mex_dep_files = dict vector
mex_deps = $(addsuffix .c, $(addprefix $(src_dir), $(mex_dep_files)))
mex_src_files = open close exec
mex_src = $(addprefix $(src_dir)$(sqlite)_,$(addsuffix .c, $(mex_src_files)))
mex_lib_names = mex mx mwlapack eng sqlite3
mex_libs = $(addprefix -l,$(mex_lib_names))
mex_ext = .mexw64

# matlab = C:\Program Files (x86)/MATLAB/R2010a Student/
matlab = C:/Progra~1/MATLAB/R2013a/
matlab_inc = -I "$(matlab)extern/include"
matlab_lib = -L "$(matlab)extern/lib/win64/microsoft/" -L "$(matlab)bin/win64/" -L "$(src_dir)$(sqlite_dir)"


all: mex

clean:
	rm -f *$(mex_ext)

testfile:
	$(cc) $(cflags) $(mex_flags)  $(src_dir)test.c -o $(subst $(src_dir), $(bld_dir), \
		$(subst .c,$(mex_ext), $(src_dir)test.c)) $(matlab_lib) $(matlab_inc) $(mex_libs)

sqlite:
	$(cc) $(cflags) $(sqlite_flags) $(src_dir)$(sqlite_dir)$(sqlite).c -o $(src_dir)$(sqlite_dir)lib$(sqlite)$(sqlite_ext)

mex: clean
	$(foreach mex_file, $(mex_src), \
		$(cc) $(cflags) $(mex_flags) $(mex_deps) $(mex_file) -o $(subst $(src_dir), $(bld_dir), \
		$(subst .c,$(mex_ext),$(mex_file))) $(matlab_lib) $(matlab_inc) $(mex_libs) &)
