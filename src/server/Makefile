NAME = ../../spion-server-embedded

SRC_NET =  ../common/net/listener.cpp ../common/net/listener.hh \
           ../common/net/poller.cpp ../common/net/poller.hh \
           ../common/net/scoped_addrinfo.cpp ../common/net/scoped_addrinfo.hh \
           ../common/net/socket.cpp ../common/net/socket.hh \
           ../common/net/types.hh

SRC_PROTOCOL =  ../common/protocol/packet.cpp ../common/protocol/packet.hh

SRC = spion.cpp spion.hh

all : $(NAME)

$(NAME) : clean
  mkdir -p $(NAME)
  mkdir -p $(NAME)/common/net
  mkdir -p $(NAME)/common/protocol
  cp $(SRC_NET) $(NAME)/common/net
  cp $(SRC_PROTOCOL) $(NAME)/common/protocol
  cp $(SRC) $(NAME)

clean :
	rm -rf $(NAME)

fclean : clean

re : fclean all
