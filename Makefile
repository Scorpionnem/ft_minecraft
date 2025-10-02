NAME = ft_minecraft

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g

GLFWARCHIVE = GLFW/build/src/libglfw3.a
GLFW_CFLAGS  = $(shell pkg-config --cflags glfw3)
GLFW_LDFLAGS = $(shell pkg-config --libs glfw3)

INCLUDES =	-I ./includes\
			-I ./GLFW/include/GLFW\
			-I glad/\
			-I ./glm/glm/\
			-I ./glm/glm/gtc/\
			-I ./includes/UI/\
			-I ./includes/UI/Elements\
			-I ./includes/Scene/Scenes\
			-I ./includes/Scene

SRCS =	glad/glad.cpp\
		src/main.cpp\
		src/Game.cpp\
		src/Window.cpp\
		src/Input.cpp\
		src/Texture.cpp\
		src/stb_image.cpp\
		src/Shader.cpp\
		src/Camera.cpp\
		src/Scene/Scenes/TitleScene.cpp\
		src/Scene/Scenes/OptionsScene.cpp\
		src/Scene/Scenes/GameScene.cpp\
		src/Scene/Scenes/MultiplayerScene.cpp\
		src/Scene/Scenes/SingleplayerScene.cpp

OBJDIR = obj
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: stb_image glfw glm $(NAME)

glfw:
	@if ls | grep -q "GLFW"; then \
		echo "\033[32;1;4mGLFW Found\033[0m"; \
	else \
		echo "\033[31;1mDownloading GLFW sources\033[0m"; \
		git clone https://github.com/glfw/glfw.git GLFW; \
		cmake -S GLFW -B GLFW/build; \
		cmake --build GLFW/build; \
	fi

stb_image:
	@if ls includes | grep -q "stb_image.h"; then \
		echo "\033[32;1;4mstb_image.h Found\033[0m"; \
	else\
		curl -o includes/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h;\
	fi

glm:
	@if ls | grep -q "glm"; then \
		echo "\033[32;1;4mGLM Found\033[0m"; \
	else \
		echo "\033[31;1;4mGLM Not Found\033[0m"; \
		echo "\033[31;1mCloning GLM from github\033[0m"; \
		git clone https://github.com/g-truc/glm.git glm; \
	fi

re: fclean all

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(GLFWARCHIVE) -o $@ $^ $(GLFW_LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $<
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(GLFW_CFLAGS) -MMD -MP -c $< -o $@

clean:
	@echo Cleaning objects
	@rm -rf $(OBJDIR)

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	./$(NAME)

.PHONY: all clean fclean re run glfw stb_image glm

-include $(DEPS)
