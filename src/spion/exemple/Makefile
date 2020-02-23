CXX = clang++

NAME = ../../../spion-server-exemple

SRC_IMGUI = ../../external/ImGui/imgui-SFML.cpp \
            ../../external/ImGui/imgui.cpp \
            ../../external/ImGui/imgui_demo.cpp \
            ../../external/ImGui/imgui_draw.cpp

SRC_LOGG =  ../../external/logg/Entry.cpp \
            ../../external/logg/I_Output.cpp \
            ../../external/logg/MetaData.cpp \
            ../../external/logg/StreamOutput.cpp \
            ../../external/logg/Timer.cpp

SRC_NET =  ../../common/net/listener.cpp \
           ../../common/net/poller.cpp \
           ../../common/net/scoped_addrinfo.cpp \
           ../../common/net/socket.cpp

SRC_PROTOCOL =  ../../common/protocol/packet.cpp \
                ../../common/protocol/string.cpp

SRC_SPION =  ../spion.cpp

SRC = $(SRC_IMGUI) $(SRC_LOGG) $(SRC_NET) $(SRC_PROTOCOL) $(SRC_SPION) main.cpp

OBJ = $(SRC:.cpp=.o)

INCLUDE_SFML_FLAGS =
INCLUDE_OTHER_FLAGS = -I../../external/ -I../../external/ImGui -I../../
WARNING_FLAGS = -Wall -Wextra
DEBUG_FLAGS = -g

LD_SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lpthread
LD_IMGUI_FLAGS = -lGL

CXXFLAGS = -std=c++17 $(WARNING_FLAGS) $(INCLUDE_OTHER_FLAGS) $(INCLUDE_SFML_FLAGS) $(DEBUG_FLAGS)
LDFLAGS = $(LD_IMGUI_FLAGS) $(LD_SFML_FLAGS)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDFLAGS)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re : fclean all
