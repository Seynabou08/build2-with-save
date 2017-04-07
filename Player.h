#ifndef PLAYER_H
#define PLAYER_H

#include "Card.h"
#include "Role.h"
#include "CityCard.h"
#include "ReferenceCard.h"
#include "EpidemicCard.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
/**#include "boost_1_63_0/boost/archive/text_oarchive.hpp"
#include "boost_1_63_0boost/archive/text_iarchive.hpp"**/

using namespace std;


class Player
{
public:

	//Constructors and Destructor methods
	Player(Map map, char p, vector<Role> &roleDeck, vector<Card*> &deck, int pCount, int id);
	Player();
	~Player();

	//toString methods 
	void printPossessions();
	void printReference();
	void displayHand();
	void displayHandWithColors();
	void displayActionsLeft();

	//Accessor and Mutator methods
	vector<Card*> getHand();
	void setHand(vector<Card*> c);
	void setRole(Role rCard);
	string getRole();
	char getPawn();
	int getId();
	int getLocation();
	int getHandSize(); //size of hand

	//actions
	void move(Map m);
	//void move(int d); //replace this later
	void flight(int a);
	void buildStation(Map* m);
	void treatDisease(Map m);
	void shareKnowledge(Player* target);
	void discoverCure(Map m);
	void activateAbility(vector<Card> &deck);

	//automatic mandatory actions
	void startTurn();
	void subtractAction();
	void concludeTurn(vector<Card*> &deck, Map map);
	void drawCards(Map map, vector<Card*> &deck, int num); //Changed so that pointer is passed
	bool checkAction();
	void discard(int a);

	//passive effects check
	void checkPassiveRole();

	//save and load functions
	void savePlayer();
	//void loadPlayer(string filename);
	void loadPlayer(ifstream filename, vector<Card*> cc, vector<Card*> rc);

	//pawn color enum type
	enum pawnColors { BLACK, GREEN, BLUE, RED, YELLOW };

private:
	int id;
	int location;
	pawnColors pawn;
	vector <Card*> cards;
	Role role;
	int actionsLeft;
	ReferenceCard rc;
	vector<EpidemicCard*> epCard;

};
#endif 