#ifndef _DECK_GUI_
#define _DECK_GUI_
#include <gtkmm.h>
#include <vector>

#include "Card.h"

class DeckGUI {
public:
	DeckGUI();
	virtual ~DeckGUI();
	Glib::RefPtr<Gdk::Pixbuf> getCardImage( const Card & );   // Returns the image for the specified card.
	Glib::RefPtr<Gdk::Pixbuf> getNullImage();                 // Returns the image to use for the placeholder.

private:
	std::vector< Glib::RefPtr< Gdk::Pixbuf > > deck;                   // Contains the pixel buffer images.
};
#endif
