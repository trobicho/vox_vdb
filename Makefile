# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/16 09:03:40 by trobicho          #+#    #+#              #
#    Updated: 2021/11/19 09:50:47 by trobicho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 				= g++
CXXFLAGS	= -std=c++14 -g
NAME			= vox_vdb

SRCS_PATH			= ./srcs
HDRS_PATH			= ./srcs
OBJS_PATH			= ./objs


ifeq ($(UNAME), Linux)
	INCS_FLAGS	= -I$(VULKAN_SDK)/include \
		-I$(GLFW3_PATH)/include -I$(GLM_PATH)/include \
		-I$(INCLUDE_PATH)
	LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan -lm -L ../my_lib_cpp/ -ltrl
else
	INCS_FLAGS	= -I$(VULKAN_SDK)/include \
		-I$(GLFW3_PATH)/include -I$(GLM_PATH)/include \
		-I$(INCLUDE_PATH)
	LDFLAGS = -L$(GLFW3_PATH)/lib -L$(VULKAN_SDK)/lib `pkg-config --static --libs glm`\
		-lvulkan -lglfw -lm -L -ltrl
endif

SRCS_NAME	=	main.cpp \
						backend/map_manager/Map_manager.cpp \
						backend/map_loader/Map_loader.cpp \
						backend/chunk_manager/Chunk_manager.cpp \
						backend/vdb_tree/Vdb_test.cpp
						
HDRS_NAME	=	backend/backend.hpp \
						backend/utils.hpp \
						backend/map_manager/Map_manager.hpp \
						backend/chunk_manager/Chunk_manager.hpp \
						backend/map_loader/Map_loader.hpp \
						backend/vdb_tree/Vdb_test.h \
						backend/vdb_tree/libvdb.h \
						backend/vdb_tree/Node.h \
						backend/vdb_tree/Root_node.h \
						backend/vdb_tree/Internal_node.h \
						backend/vdb_tree/Leaf_node.h \

OBJS_NAME	=	$(notdir $(SRCS_NAME:.cpp=.o)) 

SRCS = $(addprefix $(SRCS_PATH)/, $(SRCS_NAME))
HDRS = $(addprefix $(HDRS_PATH)/, $(HDRS_NAME))
OBJS = $(addprefix $(OBJS_PATH)/, $(OBJS_NAME))

all: $(NAME)

$(NAME): $(SRCS) $(HDRS) Makefile
	$(CC) $(CXXFLAGS) -I $(HDRS_PATH) $(INCS_FLAGS) $(SRCS) $(LDFLAGS) -o $(NAME)

$(OBJS_PATH)/%.o: $(SRCS_PATH)/%.cpp $(HDRS) Makefile
	@mkdir $(OBJS_PATH) 2> /dev/null || true
	@echo $fg[red] $< -> $@
	@$(CC) $(CXXFLAGS) -I $(HDRS_PATH) $(INCS_FLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
