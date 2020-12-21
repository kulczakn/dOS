# Output file
client_exe 	= client
server_exe  = server

# Flags
compiler_flags = -lpthread

# Modules
cddp_lib    = build/cddp.o

# Paths
include		= -I./include
src			= ./src
build		= build/*.o

# Building
directories = build
modules 	= cddp
flags 		= ${include} ${compiler_flags}

all: ${directories} client # server

client: src/client_main.c ${modules}
	gcc src/client_main.c ${build} -o ${client_exe} ${flags}

# server: src/server_main.c ${modules}
# 	gcc src/server_main.c ${build} -o ${server_exe} ${flags}

# Cleaning
clean:
	rm -f build/* ${client_exe} ${server_exe}

# Module Building
cddp: src/cddp.c include/*
	gcc -c src/cddp.c -o ${cddp_lib} ${flags}

build: 
	mkdir build