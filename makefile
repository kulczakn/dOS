# Output file
client_exe 	= client
server_exe  = server

# Flags
# compiler_flags = 

# Modules
mem 	= build/mem.o
mtx 	= build/mtx.o
sem 	= build/sem.o
sys 	= build/sys.o
tsk		= build/tsk.o

# Paths
include		= -I./include
src			= ./src
build		= build/*.o

# Building
modules = ${mem} ${tsk} ${sys} # ${mtx} ${sem} 
flags 	= ${include} # ${compiler_flags}

all: client # server

client: src/client_main.c ${modules}
	gcc src/client_main.c ${build} -o ${client_exe} ${flags}

server: src/server_main.c ${modules}
	gcc src/server_main.c ${build} -o ${server_exe} ${flags}

# Cleaning
clean:
	rm -f build/* ${client_exe} ${server_exe}

# Module Building
${mem}: src/syn_mem.c include/*
	gcc -c src/syn_mem.c -o ${mem} ${flags}

${sys}: src/syn_sys.c include/*
	gcc -c src/syn_sys.c -o ${sys} ${flags}

${tsk}: src/syn_tsk.c include/*
	gcc -c src/syn_tsk.c -o ${tsk} ${flags}