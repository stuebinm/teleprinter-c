cmake_minimum_required (VERSION 2.6)
project (teleprinter-c)

 # executable
add_executable(teleprinter 
	src/main.c
	src/teleprinter.c
	src/document.c
	src/parsing.c
	src/charv.c
	src/macrostack.c
	src/assembly.c
	src/error.c
)

