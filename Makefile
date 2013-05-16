# makefile

OFILES = tictactoe.o exPlayer.o ohPlayer.o

CC = g++
CCFLAGS = -ansi -O -g
LIBFLAGS = -lstdc++
LOADFLAGS =

.SUFFIXES:	.cpp
.cpp.o:
	-$(CC) $(CCFLAGS) $(LOADFLAGS) -c $<

tictactoe.o: tictactoe.cpp exPlayer.cpp ohPlayer.cpp player.h
	-$(CC) $(CCFLAGS) $(LOADFLAGS) -c $<

play: $(OFILES) Makefile
	-$(CC) $(CCFLAGS) -o $@ $(OFILES)

#cleaning up
clean:	
	rm -f $(OFILES) play
