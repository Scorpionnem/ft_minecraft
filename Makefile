NAME :=	shaderpixel

CXX := c++
CXXFLAGS :=	-g -MP -MMD -std=c++20# -Wall -Wextra -Werror

LIB_DIR :=	lib/
INC_DIR :=	inc/
SRC_DIR :=	src/
OBJ_DIR :=	.obj/

MATH_LIB_DIR :=	$(LIB_DIR)math
STB_LIB_DIR :=	$(LIB_DIR)stb_image

INCLUDE_DIRS :=	-I$(INC_DIR) -I$(LIB_DIR) -I$(MATH_LIB_DIR)/inc
SDL_CFLAGS :=	$(shell sdl2-config --cflags)
SDL_LIBS :=		$(shell sdl2-config --libs)
LFLAGS :=		$(SDL_LIBS) -lGL

SRCS :=	src/main.cpp						\
		src/App.cpp							\
		src/platform/Window.cpp				\
		src/platform/Input.cpp				\
		src/render/Shader.cpp				\
		src/render/FrameBuffer.cpp			\
		src/loader/mesh/OBJLoader.cpp		\
		src/loader/texture/STBLoader.cpp	\
		src/ui/UI.cpp	\

OBJS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.o)
DEPS :=	$(SRCS:%.cpp=$(OBJ_DIR)%.d)

all: $(MATH_LIB_DIR) $(STB_LIB_DIR) $(NAME)

$(MATH_LIB_DIR): | $(LIB_DIR)
	git clone git@github.com:Scorpionnem/graphics_math.git $(MATH_LIB_DIR)

$(STB_LIB_DIR): | $(LIB_DIR)
	@if ls external/stb_image | grep -q "stb_image.h"; then \
		printf ""; \
	else\
		mkdir -p $(STB_LIB_DIR); \
		curl -o $(STB_LIB_DIR)/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h;\
	fi

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

$(OBJ_DIR)%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(SDL_CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re

-include $(DEPS)
