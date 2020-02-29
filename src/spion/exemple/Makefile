CXX = clang++

NAME = spion-server-exemple

SPION_ROOT_SRC_FOLDER = ..

# SRC_IMGUI = ../../external/ImGui/imgui-SFML.cpp \
#             ../../external/ImGui/imgui.cpp \
#             ../../external/ImGui/imgui_demo.cpp \
#             ../../external/ImGui/imgui_draw.cpp

SRC_LOGG =  $(SPION_ROOT_SRC_FOLDER)/../external/logg/Entry.cpp \
            $(SPION_ROOT_SRC_FOLDER)/../external/logg/I_Output.cpp \
            $(SPION_ROOT_SRC_FOLDER)/../external/logg/MetaData.cpp \
            $(SPION_ROOT_SRC_FOLDER)/../external/logg/StreamOutput.cpp \
            $(SPION_ROOT_SRC_FOLDER)/../external/logg/Timer.cpp

SRC_NET =  $(SPION_ROOT_SRC_FOLDER)/../common/net/listener.cpp \
           $(SPION_ROOT_SRC_FOLDER)/../common/net/poller.cpp \
           $(SPION_ROOT_SRC_FOLDER)/../common/net/scoped_addrinfo.cpp \
           $(SPION_ROOT_SRC_FOLDER)/../common/net/socket.cpp

SRC_PROTOCOL =  $(SPION_ROOT_SRC_FOLDER)/../common/protocol/string.cpp

SRC_SPION =  $(SPION_ROOT_SRC_FOLDER)/Client.cpp \
             $(SPION_ROOT_SRC_FOLDER)/Server.cpp \
             $(SPION_ROOT_SRC_FOLDER)/spion.cpp

SRC = $(SRC_LOGG) $(SRC_NET) $(SRC_PROTOCOL) $(SRC_SPION) main.cpp

OBJ = $(SRC:.cpp=.o)

# INCLUDE_SFML_FLAGS = -I../../external/ImGui
INCLUDE_OTHER_FLAGS = -I../../external/ -I../../
WARNING_FLAGS = -Wall -Wextra
DEBUG_FLAGS = -g

# LD_SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
# LD_IMGUI_FLAGS = -lGL

CXXFLAGS = -std=c++17 $(WARNING_FLAGS) $(INCLUDE_OTHER_FLAGS) $(DEBUG_FLAGS)
LDFLAGS = -lpthread

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
