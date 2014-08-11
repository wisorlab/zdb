cc = gcc
cflags = -std=c11 -m32 -Wall
sqlite = sqlite3
sqlite_ext = .dll
sqlite_flags = -shared
sqlite_dir = lib/
src_dir = src/

bld_dir = +build/

mex_flags = -DMATLAB_MEX_FILE -shared
mex_dep_files = dict vector
mex_deps = $(addsuffix .c, $(addprefix $(src_dir), $(mex_dep_files)))
mex_src_files = open close exec
mex_src = $(addprefix $(src_dir)$(sqlite)_,$(addsuffix .c, $(mex_src_files)))
mex_lib_names = mex mx mwlapack eng sqlite3
mex_libs = $(addprefix -l,$(mex_lib_names))
mex_ext = .mexw32

matlab = C:\Program Files (x86)/MATLAB/R2010a Student/
matlab_inc = -I "$(matlab)extern/include"
matlab_lib = -L "$(matlab)extern/lib/win32/microsoft/" -L "$(matlab)bin/win32/" -L "$(src_dir)$(sqlite_dir)"


all: mex

sqlite:
	$(cc) $(cflags) $(sqlite_flags) -c $(src_dir)$(sqlite_dir)$(sqlite).c -o $(src_dir)$(sqlite_dir)$(sqlite)$(sqlite_ext)

mex:
	$(foreach mex_file, $(mex_src), \
		$(cc) $(cflags) $(mex_flags) $(mex_deps) $(mex_file) -o $(subst $(src_dir), $(bld_dir), \
		$(subst .c,$(mex_ext),$(mex_file))) $(matlab_lib) $(matlab_inc) $(mex_libs) &)
