cmake_minimum_required (VERSION 2.6)
project (teleprinter-c)

 # executable
add_executable(teleprinter 
	src/main.c
	src/document.c
	src/layer.c
	src/parsing.c
	src/charv.c
	src/commands.c
	src/error.c
)

