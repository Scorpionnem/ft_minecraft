NAME :=	ft_minecraft

CXX := c++
CXXFLAGS :=	-g -MP -MMD -std=c++20 -Wall -Wextra -Werror -O3 # -fsanitize=address -fno-omit-frame-pointer

LIB_DIR :=	lib/
LIBMBL_PATH := $(LIB_DIR)mbl/
INC_DIR :=	inc/
SRC_DIR :=	src/
OBJ_DIR :=	.obj/

LIBMBL := $(LIBMBL_PATH)libmbl.a

INCLUDE_DIRS :=	-I$(INC_DIR) -I$(LIB_DIR) -I $(LIBMBL_PATH)inc
SDL_CFLAGS :=	$(shell sdl2-config --cflags)
SDL_LIBS :=		$(shell sdl2-config --libs)
LFLAGS :=		$(SDL_LIBS) -lGL

SRCS :=	src/main.cpp									\
		src/app/Client.cpp								\
		src/app/Server.cpp								\
		src/app/scene/SceneManager.cpp					\
		src/app/scenes/MainScene.cpp					\
		src/app/scenes/MultiplayerScene.cpp				\
		src/app/scenes/SingleplayerScene.cpp			\
		src/app/scenes/GameScene/init.cpp				\
		src/app/scenes/GameScene/render.cpp				\
		src/app/scenes/GameScene/unload.cpp				\
		src/app/scenes/GameScene/update.cpp				\
		src/app/scenes/GameScene/update/camera.cpp		\
		src/app/scenes/GameScene/update/net.cpp			\
		src/game/world/Chunk.cpp						\
		src/game/world/World.cpp						\
		src/game/world/ClientWorld.cpp					\
		src/game/world/ServerWorld.cpp					\
		src/game/world/Block.cpp						\
		src/game/world/generation/NoiseGenerator.cpp	\
		src/game/world/render/mesh/ChunkMesher.cpp		\
		src/game/world/render/ChunkRender.cpp			\

OBJS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.d)

all: $(LIBMBL) $(NAME)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIBMBL):
	@make -C $(LIBMBL_PATH) all --no-print-directory

$(NAME): $(OBJS) $(LIBMBL)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LIBMBL) $(LFLAGS)

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(SDL_CFLAGS) -c $< -o $@

clean:
	@make -C $(LIBMBL_PATH) clean --no-print-directory
	rm -rf $(OBJ_DIR)

fclean: clean
	@make -C $(LIBMBL_PATH) fclean --no-print-directory
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
