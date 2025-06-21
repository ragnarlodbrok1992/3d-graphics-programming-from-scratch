SDL2INC="D:/libraries/SDL2-2.28.2/include/"
SDL2LIB="D:/libraries/SDL2-2.28.2/lib/x64/"

win_all: src/main.c win_tools
	@cp "$(SDL2LIB)"\SDL2.dll .
	@cl /W4 /sdl /nologo \
		/std:c17 \
		/Zi \
		/EHsc \
		/I$(SDL2INC) \
		/Fe3drenderer.exe \
		src/display.c \
		src/vector.c \
		src/main.c \
		src/mesh.c \
		src/triangle.c \
		src/array.c \
		shell32.lib \
		User32.lib \
		/link /LIBPATH:"$(SDL2LIB)" \
					SDL2.lib SDL2main.lib

win_tools: src/tools/tools_main.c
	@cl /W4 /nologo \
		/std:c17 \
		/Zi \
		/EHsc \
		src/tools/tools_main.c \
		/link /OUT:render_tools.exe

win_run:
	3drenderer.exe

win_clean:
	@del *.obj *.exe *.ilk *.pdb *.dll *.lib *~

test: win_tools
	@render_tools.exe assets\test\cube.obj
