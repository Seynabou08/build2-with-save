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
	void setEventCard(string EventCard, int a);

	string getEventCard();


	/* void passiveAbility(char disease, int city);
	void activeAbility(vector<Card> &deck, int city);
	*/

	//The seven different EventCards 
	/* void contingencyPlannerAct(vector<Card> &deck);
	void dispatcherAct(vector<Card> &hand, int city, Map m);
	void medicAb(char disease);
	void operationsExpertAb(char disease);
	void quarantineExpertAb();
	void researcherAb(Card c);
	void scientistAb(char disease);
	*/

private:
	Card cCard; //contigency card
	string assignedEventCard;
};
#endif
