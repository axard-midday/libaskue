# 
# Сборка для libaskue 2.0.3
#

project_name := libaskue
majorVersion := 2
minorVersion := 0.14
fullVersion := $(majorVersion).$(minorVersion)
source_dirs := ../src
dest_dir := ../bin

source_files := $(wildcard $(addsuffix /*.c, $(source_dirs)))

object_files := $(notdir $(source_files))
object_files := $(object_files:.c=.o)

link_flags := -lsqlite3
compile_flags := -std=c99 -fPIC -Wall
soname_flag := -Wl,-soname,$(project_name).so.$(majorVersion)

CC := gcc-3.4

output := $(addprefix $(dest_dir), $(project_name) )
output := $(addsuffix .so., $(project_name))
output := $(addsuffix $(fullVersion), $(output))

all: $(project_name) Clean_Object_Files Move_BinDir

Move_BinDir:
	mv $(output) $(addsuffix $(output), $(addsuffix /, $(dest_dir)))

Clean_Object_Files:
	rm *.o
	
$(project_name): $(object_files)
	$(CC) $^ -o $(output) $(soname_flag) -pipe -shared $(link_flags)

%.o:%.c 
	$(CC) $^ -c $(compile_flags) $(addprefix -I, $(source_dirs)) -pipe 
	
VPATH := $(source_dirs)

.PHONY: all Clean_Object_Files Move_BinDir
