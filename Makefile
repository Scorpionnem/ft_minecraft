NAME :=	ft_minecraft

CXX := c++
CXXFLAGS :=	-g -MP -MMD -std=c++20 # -Wall -Wextra -Werror

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

SRCS :=	src/main.cpp						\
		src/App.cpp							\

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
