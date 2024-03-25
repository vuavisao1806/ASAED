# This is Makefile I learned from my big senior and I adapted it to suit myself and this project 

CPP_FILES = main.cpp                         \
            asaed/constants.cpp              \
            asaed/gameconfig.cpp             \
            asaed/globals.cpp                \
            asaed/main.cpp                   \
            control/controller.cpp           \
            control/input_manager.cpp        \
            control/keyboard_config.cpp      \
            control/keyboard_manager.cpp     \
            math/rect.cpp                    \
            math/rectf.cpp                   \
            math/size.cpp                    \
            math/sizef.cpp                   \
            math/vector.cpp                  \
            object/player.cpp                \
            object/tile_manager.cpp          \
            object/tile_map.cpp              \
            object/tile_parser.cpp           \
            object/tile_set.cpp              \
            object/tile.cpp                  \
            sprite/sprite_data.cpp           \
            sprite/sprite_manager.cpp        \
            sprite/sprite.cpp                \
            util/log.cpp                     \
            util/reader_assert.cpp           \
            util/reader_data.cpp             \
            util/reader_machine.cpp          \
            video/canvas.cpp                 \
            video/color.cpp                  \
            video/compositor.cpp             \
            video/drawing_context.cpp        \
            video/painter.cpp                \
            video/renderer.cpp               \
            video/surface_creator.cpp        \
            video/surface.cpp                \
            video/texture_manager.cpp        \
            video/texture.cpp                \
            video/video_system.cpp           \
            video/viewport.cpp               \



#CC specifies which compiler we're using
CC = g++

SDL2_PATH = D:\libs\SDL2-2.28.5\x86_64-w64-mingw32
SDL2_IMAGE_PATH = D:\libs\SDL2_image-2.8.1\x86_64-w64-mingw32

# $ to reference to the path, if doesn't use this, it like -I SDL2_PATH\include\SDl2 instead of D:\libs\SDL2-2.28.5\x86_64-w64-mingw32\....
INCLUDE_PATHS = -I$(SDL2_PATH)\include\SDL2         \
                -I$(SDL2_IMAGE_PATH)\include\SDL2   \
                -I./src                             \

LIBRARY_PATHS = -L$(SDL2_PATH)\lib          \
                -L$(SDL2_IMAGE_PATH)\lib    \

LINKER_FLAGS = -lmingw32        \
               -lSDL2main       \
               -lSDL2           \
               -lSDL2_image     \

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -std=c++17 -Wall -Wextra

GFLAGS = -g

OBJ_FILES = $(addsuffix .o, $(basename $(CPP_FILES)))
OBJDIR := bin/obj

#OBJS specifies which files to compile as part of the project
OBJS := $(addprefix $(OBJDIR)/, $(OBJ_FILES))


#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ASAED.exe
### Move the executable to ./data to linked with dynamic link library
OBJ_NAME := $(addprefix ./data/, $(OBJ_NAME))


# Why -c ?
#This is essential (!!!) because it allows Make to correctly track dependencies and rebuild only the necessary object files when source files change, without unnecessarily relinking the entire program.
$(OBJDIR)/%.o : src/%.cpp
	-@mkdir -p $(@D)
	$(CC) -c $(GFLAGS) $(INCLUDE_PATHS) $(COMPILER_FLAGS) $< -o $@

#This is the target that compiles our executable
.PHONY : all
all : $(OBJ_NAME)

$(OBJ_NAME) : $(OBJS)
	$(CC) $(GFLAGS) $(OBJS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

$(OBJS) : | $(OBJDIR)

$(OBJDIR) :
	-@mkdir -p $(OBJDIR)

.PHONY : clean
clean:
	-rm -rf ./bin/obj/
	-rm ./data/ASAED.exe