all:
	cp src/*.vert bin
	cp src/*.frag bin
	gcc src/main.c -o ./bin/JuliaGL -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi
#order mattered for the libraries 