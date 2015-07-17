CXX= g++
CXXFLAGS = -Wall -O -g `pkg-config gtkmm-2.4 --cflags` -std=c++0x
LDFLAGS=`pkg-config gtkmm-2.4 --libs`
OBJS = Straights.o Subject.o View.o Game.o Card.o Player.o Deck.o ComputerPlayer.o HumanPlayer.o Controller.o DeckGUI.o
DEPENDS = ${OBJECTS:.o=.d}
EXEC = straights

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) $(LDFLAGS) -o $(EXEC)

clean:
	rm -rf ${DEPENDS} $(EXEC) $(OBJS)

-include ${DEPENDS}
