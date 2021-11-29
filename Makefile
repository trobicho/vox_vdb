# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: trobicho <trobicho@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/16 09:03:40 by trobicho          #+#    #+#              #
#    Updated: 2021/11/29 18:36:46 by trobicho         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 				= g++
CXXFLAGS	= -std=c++14 -g -Og
NAME			= vox_vdb

SRCS_PATH			= ./srcs
HDRS_PATH			= ./srcs
OBJS_PATH			= ./objs


ifeq ($(UNAME), Linux)
	INCS_FLAGS	= -I$(VULKAN_SDK)/include \
		-I$(GLFW3_PATH)/include -I$(GLM_PATH)/include \
		-I$(INCLUDE_PATH)
	LDFLAGS = -L$(VULKAN_SDK)/lib `pkg-config --static --libs glfw3` -lvulkan -lm -L ../my_lib_cpp/ -ltrl
	#LDFLAGS = -lm -L ../my_lib_cpp/ -ltrl -lpthread
else 
	INCS_FLAGS	= -I$(VULKAN_SDK)/include \
		-I$(GLFW3_PATH)/include -I$(GLM_PATH)/include \
		-I$(INCLUDE_PATH)
	LDFLAGS = -L$(GLFW3_PATH)/lib -L$(VULKAN_SDK)/lib `pkg-config --static --libs glm`\
		-lvulkan -lglfw -lm -L -ltrl
endif

SRCS_NAME	=	main_glfw.cpp \
						backend/map_manager/Map_manager.cpp \
						backend/map_loader/Map_loader.cpp \
						backend/chunk_manager/Chunk_manager.cpp \
						backend/vdb_tree/Vdb_test.cpp \
						frontend/map_generator/Basic_terrain.cpp \
						frontend/noise/Noise.cpp \
						frontend/noise/perlin.cpp
						
HDRS_NAME	=	backend/backend.hpp \
						backend/utils.hpp \
						backend/map_manager/Map_manager.hpp \
						backend/chunk_manager/Chunk_manager.hpp \
						backend/map_loader/Map_loader.hpp \
						backend/chunk_map/Chunk_map.hpp \
						backend/vdb_tree/Vdb_test.h \
						backend/vdb_tree/libvdb.h \
						backend/vdb_tree/Node.h \
						backend/vdb_tree/Root_node.h \
						backend/vdb_tree/Internal_node.h \
						backend/vdb_tree/Leaf_node.h \
						backend/vdb_tree/Generate_node.hpp \
						backend/front_interfaces/Map_sampler.hpp \
						frontend/map_generator/Basic_terrain.cpp \
						frontend/noise/Noise.h \
						frontend/noise/perlin.h

HDRS_FRONTEND_NAME	=	Basic_terrain.hpp \

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
