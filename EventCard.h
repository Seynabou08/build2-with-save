#ifndef EVENTCARD_H
#define EVENTCARD_H

//#include <string>
#include <vector>
#include "Card.h"
#include "Map.h"
using namespace std;

class EventCard : public Card
{
public:
	vector<EventCard*> EventCardCards;

	//Constructors and Destructor
	EventCard(string n, int i);
	EventCard();
	~EventCard();

	void printEventCard();

	//Accessor and Mutator methods
	void setEvent(string eventChosen, int a);

	string getEventCard();

private:
	Card eCard;
	string assignedEvent;
	string type;
};
#endif
