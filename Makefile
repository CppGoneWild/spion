#SERVER_EMBD = spion-server-embedded
SERVER_EX   = src/spion/spion-server-exemple

all : $(SERVER_EX) #$(SERVER_EMBD)

$(SERVER_EX) :
	make -C src/server/exemple -f Makefile

#$(SERVER_EMBD) :
#	make -C src/server/ -f Makefile

clean :
	make clean -C src/server/exemple -f Makefile
#	make clean -C src/server/ -f Makefile

fclean : clean
	make fclean -C src/server/exemple -f Makefile

re : fclean all
