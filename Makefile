CLIENT      = spion-client
#SERVER_EMBD = spion-server-embedded
SERVER_EX   = spion-server-exemple

all : $(CLIENT) $(SERVER_EX) #$(SERVER_EMBD)

$(CLIENT) :
	make -C src/client -f Makefile

$(SERVER_EX) :
	make -C src/server/exemple -f Makefile

#$(SERVER_EMBD) :
#	make -C src/server/ -f Makefile

clean :
	make clean -C src/client -f Makefile
	make clean -C src/server/exemple -f Makefile
#	make clean -C src/server/ -f Makefile

fclean : clean
	make fclean -C src/client -f Makefile
	make fclean -C src/server/exemple -f Makefile

re : fclean all
