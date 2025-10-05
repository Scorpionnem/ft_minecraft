NAME = ft_minecraft

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++17 -g

GLFWARCHIVE = external/GLFW/build/src/libglfw3.a
GLFW_CFLAGS  = $(shell pkg-config --cflags glfw3)
GLFW_LDFLAGS = $(shell pkg-config --libs glfw3)

INCLUDES =	-I ./includes\
			-I ./external\
			-I ./external/GLFW/include/GLFW\
			-I ./external/glad/\
			-I ./external/GLM/glm/\
			-I ./external/GLM/glm/gtc/\
			-I ./includes/UI/\
			-I ./includes/UI/Elements\
			-I ./includes/Scene/Scenes\
			-I ./includes/Scene\
			-I ./includes/Renderer

SRCS =	external/glad/glad.cpp\
		src/stb_image.cpp\
		src/main.cpp\
		src/Game.cpp\
		src/Window.cpp\
		src/Input.cpp\
		src/Renderer/Shader.cpp\
		src/Renderer/Texture.cpp\
		src/Camera.cpp\
		src/Scenes/TitleScene.cpp\
		src/Scenes/OptionsScene.cpp\
		src/Scenes/GameScene.cpp\
		src/Scenes/MultiplayerScene.cpp\
		src/Scenes/SingleplayerScene.cpp\
		src/Scenes/MultiplayerNewScene.cpp\
		src/UI/UIElement.cpp\
		src/UI/Elements/ImprovedButton.cpp\
		src/UI/Elements/ImprovedText.cpp\
		src/UI/Elements/ImprovedBackgroundImage.cpp\
		src/UI/Elements/ImprovedImage.cpp
		

OBJDIR = obj
EXTERNALDIR = external
OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEPS = $(SRCS:%.cpp=$(OBJDIR)/%.d)

all: $(EXTERNALDIR) stb_image glfw glm $(NAME)

$(EXTERNALDIR):
	@mkdir external

glfw:
	@if ls external | grep -q "GLFW"; then \
		echo "\033[32;1;4mGLFW Found\033[0m"; \
	else \
		echo "\033[31;1mDownloading GLFW sources\033[0m"; \
		git clone https://github.com/glfw/glfw.git external/GLFW; \
		cmake -S external/GLFW -B external/GLFW/build; \
		cmake --build external/GLFW/build; \
	fi

stb_image:
	@if ls external | grep -q "stb_image.h"; then \
		echo "\033[32;1;4mstb_image.h Found\033[0m"; \
	else\
		echo "\033[31;1mDownloading stb_image.h\033[0m"; \
		curl --silent -o external/stb_image.h https://raw.githubusercontent.com/nothings/stb/master/stb_image.h;\
		echo "\033[31;1mDownloaded stb_image.h\033[0m"; \
	fi

glm:
	@if ls external | grep -q "GLM"; then \
		echo "\033[32;1;4mGLM Found\033[0m"; \
	else \
		echo "\033[31;1mDownloading GLM sources\033[0m"; \
		git clone https://github.com/g-truc/glm.git external/GLM; \
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

dclean: fclean
	@echo Cleaning external libraries
	@rm -rf external/

fclean: clean
	@echo Cleaning $(NAME)
	@rm -rf $(NAME)

run: $(NAME)
	@./$(NAME)

.PHONY: all clean fclean re run glfw stb_image glm

-include $(DEPS)
