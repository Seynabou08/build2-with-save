#include "EventCard.h"

EventCard::EventCard(string n, int i) :Card(n, 'r', i)
{
	eCard = Card(n, 'r', i);
}

EventCard::EventCard() : Card("null", 'r', 0)
{
	eCard = Card("null", 'r', 0);
}

EventCard::~EventCard(){}

void EventCard::setEvent(string eventChosen, int a)
{
	this->assignedEvent = eventChosen;
	this->setName("Event Card");
	this->setId(a);
}

string EventCard::getEventCard()
{
	return this->assignedEvent;
}


void EventCard::printEventCard()
{
	cout << this->getName() << this->getId() << endl;
}

