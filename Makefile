CXX = g++
CXXFLAGS = -g -Wall -MMD -std=c++0x
OBJECTS = Straights.o View.o Game.o Card.o Command.o Player.o Deck.o ComputerPlayer.o HumanPlayer.o Controller.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = straights

${EXEC}	: ${OBJECTS}
		${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

clean	:
		rm -rf ${DEPENDS} ${OBJECTS} ${EXEC}

-include ${DEPENDS}