#include "Map.h"
#include "EventCard.h"
#include "InfectionDeck.h"
#include "EpidemicCard.h"
#include <iostream>
#include <algorithm>
#include "Player.h"

using namespace std;

int main(int argc, char* argv[])
{
	// Initialiing the map 
	Map newMap = Map(48);
	newMap.startGame();

	// Acessing the save from which we load the game
	ifstream file("save.txt");

	// newMap.loadMap(ifstream("save.txt")); //LOADING DOESNT WORK


	//saving the file in visual studio folder
	newMap.saveMap(); 

	//displaying the map
	newMap.showMap();



	//Place the research station in Atlanta.

	City atlanta = newMap.getCity(7);
	atlanta.setResearchCenter(true);
	

	//Placing the 4 cure markers, side	up near the discovered cure indicators.
	cout << "CURES: " << newMap.blueCure << ", " << newMap.yellowCure << ", " << newMap.whiteCure << ", " << newMap.redCure << endl;



	
	cout << "How many players do you want to play?" << endl;
	int a = 0;
	int playerNum;
	while (a > 4 || a == 0)
	{
		cin >> a;
	}

	switch (a)
	{
	case 2:
		cout << "you have selected 2" << endl;
		playerNum = 2;
		break;
	case 3:
		cout << "you have selected 3" << endl;
		playerNum = 3;
		break;
	case 4:
		cout << "you have selected 4" << endl;
		playerNum = 4;
		break;
	}	

	vector<Player> players;
	int cardNum;
		
	if (playerNum == 2) {
		cardNum = 4;
		Player player1 = Player();
		Player player2 = Player();
		players.push_back(player1);
		players.push_back(player2);
	}
	else if (playerNum == 3) {
		cardNum = 3;
		Player player1 = Player();
		Player player2 = Player();
		Player player3 = Player();

		players.push_back(player1);
		players.push_back(player2);
		players.push_back(player3);
	}
	else if (playerNum == 4) {
		cardNum = 2;
		Player player1 = Player();
		Player player2 = Player();
		Player player3 = Player();
		Player player4 = Player();
		players.push_back(player1);
		players.push_back(player2);
		players.push_back(player3);
		players.push_back(player4);
	}

	// Setting up the player deck
	vector<Card*> playerDeck;
	
		
	for (int i = 0; i < newMap.cities.size(); i++) {
		CityCard* ccard = new CityCard(newMap.cities[i]);
		Card* card = new Card();
		card = ccard;

		playerDeck.push_back(card);
	}

	//Generating event card to add to player deck
	string eventNames[] = { "Airlift", "One Quiet Night", "Resilient population",
		"Government Grant", "Forcast"};

	EventCard* eventCard = new EventCard();
	for (int i = 0; i < 5; i++)
	{
		eventCard->setEvent(eventNames[i], i);
		playerDeck.push_back(eventCard);
	}

	//Generating epidemic card to add to player deck
	
	EpidemicCard* epidemicCard = new EpidemicCard();
	for (int i = 0; i < 5; i++)
	{
		epidemicCard->setEpidemic(i);
		playerDeck.push_back(epidemicCard);
	}
	

	// GENERATING ROLE DECK
	vector<Role*> roleDeck;

	string roleNames[] = { "Contingency Planner","Dispatcher", "Medic",
		"Operation Expert", "Quarantine Specialist", "Researcher", "Scientist" };
	
	Role* roleCard = new Role();
	for (int i = 0; i < 7; i++)
	{
		roleCard->setRole(roleNames[i],i+1);
		roleDeck.push_back(roleCard);
	}


	//Shuffling the Player cards randomly
	random_shuffle(playerDeck.begin(), playerDeck.end());
	random_shuffle(roleDeck.begin(), roleDeck.end());

	
	//Infection deck initialization
	cout << "INITIALIZING DECK" << endl;
	InfectionDeck ideck = InfectionDeck(newMap.cities);
	ideck.init(48);
	ideck.initialInfection();


	for (int i = 0; i < playerNum; i++) {
	
		Player* player = &players.at(i);

		player->drawCards(playerDeck, cardNum);	//TODO Needs to use pointer so that copy of deck isn't passed

		player->setRole(*roleDeck.back());

		roleDeck.pop_back();
	}

	
	//Displaying player locations
	for(int i=0; i<playerNum; i++) cout << "Player " << i+1 << " at" << newMap.cities[players.at(i).getLocation()]->getName() << endl;
	
	
	bool gameover = false;

	// This loop ensures that each player gets a turn one after te other followed by infection turns
	while (!gameover) {   //If gameover is true we will exit the loop and that ends the game

		for (int i = 0; i < playerNum; i++) {
			players.at(i).startTurn();
			cout << "------------------PLAYER "<< i+1 <<" TURN------------------" << endl;
			while (players.at(i).checkAction())
			{
				//Displaying the possible actions the player can make
				cout << "Player " << i+1 <<" is at " << newMap.cities[players.at(i).getLocation()]->getName() << endl;
				cout << "Player " << i+1 << ": Choose your action..." << endl;

				players.at(i).displayActionsLeft();

				//PLAYER ACTIONS
				//TODO REMOVE ABILIY TO MOVE/FLY TO EPIDEMIC CARDS
				cout << "1. Drive/Ferry" << endl;
				cout << "2. Direct Flight" << endl;
				cout << "3. Charter Flight" << endl;
				cout << "4. Shuttle Flight" << endl;
				cout << "5. Build a Research Station" << endl;
				cout << "6. Treat Disease" << endl;
				cout << "7. Share Knowledge" << endl;
				cout << "8. Discover a Cure" << endl;
				cout << "9. Play event card" << endl;

				char action;
				cin >> action;

				switch (action) {
				case '1':	//Drive/Ferry
				{
					newMap.showCity(players.at(i).getLocation());
					players.at(i).move(newMap);

					break;
				}
				case '2':	//Direct Flight: Discard a City card to move to the city named on the card.
				{
					players.at(i).displayHand();

					int cardInt = -1;
					while (cardInt == -1 || cardInt >= players.at(i).getHandSize()) {
						cin >> cardInt;
					}

					players.at(i).flight(players.at(i).getHand()[cardInt]->getId());

					players.at(i).discard(cardInt);
					break;
				}
				case '3':	//Charter Flight: Discard the City card that matches the city you are in to move to any city.
				{
					//check for matching card
					bool hasMatchingCard = false;
					int matchingCardIndex;
					for (int j = 0; j < players.at(i).getHandSize(); j++) {
						if (players.at(i).getHand()[j]->getId() == players.at(i).getLocation()) {
							hasMatchingCard = true;
							matchingCardIndex = j;
						}
					}

					if (hasMatchingCard) {

						//SHOW ALL CITIES AND IDs
						for (int k = 0; k < newMap.cities.size(); k++) {
							cout << "(" << k << ") " << newMap.cities[k]->getName() << endl;
						}

						cout << "Choose a city to fly to" << endl;

						int cityID;
						cin >> cityID;
						players.at(i).flight(cityID);
						players.at(i).discard(matchingCardIndex);
					}
					else {
						cout << "You cannot take a charter flight as you do not have a card matching the city you are currently on" << endl;
					}

					break;
				}
				case '4': //Shuttle Flight	Move from a city with a research station to any other city that has a research station.
				{
					City* location = newMap.accessCity(players.at(i).getLocation());
					vector<int> validCities; //indexes of cities with research stations

					if (location->researchCenter == true)
					{

						for (int j = 0; j < 47; j++)
						{
							City* newLoc = newMap.accessCity(i);
							if (newLoc->researchCenter == true)
							{
								cout << newLoc->accessCity() << " : " << newLoc->getName();
								validCities.push_back(newLoc->accessCity());
							}
						}

						bool isValidCity = false;

						while (!isValidCity) {
							int newLocation;
							cout << "Enter the ID of the city you want to move to:" << endl;
							cin >> newLocation;

							//verify if valid city
							for (int k = 0; k < validCities.size(); k++) {
								if (newLocation == validCities[k]) {
									isValidCity = true;
								}
							}

							if (isValidCity) {
								players.at(i).flight(newLocation);
								break;
							}
						}
					}
					break;
				}
				case '5': //Building a Research Station

				{
					bool hasMatchingCard = false;
					int matchingCardIndex;
					for (int j = 0; j < players.at(i).getHandSize(); j++) {
						if (players.at(i).getHand()[j]->getId() == players.at(i).getLocation()) {
							hasMatchingCard = true;
							matchingCardIndex = j;
						}
					}

					if (hasMatchingCard) {

						City* location = newMap.accessCity(players.at(i).getLocation());
						location->researchCenter = true;

						players.at(i).treatDisease(newMap);

						players.at(i).discard(matchingCardIndex);
						cout << "A research station was built in " << location->getName() << endl;
					}
					else {
						cout << "To build a research station the card matching your location is required" << endl;
					}

					break;
				}

				case '6':	//Treating a disease

				{
					City* location = newMap.accessCity(players.at(i).getLocation());

					players.at(i).treatDisease(newMap); //TODO Might have to add disease cubes back to supply

					break;
				}

				case '7': //Sharing knowledge

				{
					cout << "With which player do you want to share knowledge?";
					int a;
					cin >> a;
					Player* buddy = &players.at(a);
					players.at(i).shareKnowledge(*buddy);
					break;
				}

				case '8'://Discover a Cure
				{
					if (newMap.accessCity(players.at(i).getLocation())->researchCenter) { //check to see if player is at research station
						players.at(i).displayHandWithColors();

						int cardInt1, cardInt2, cardInt3, cardInt4, cardInt5;

						cout << "Enter the ID of the cards you want to discard: (5 of same color)" << endl;

						cin >> cardInt1 >> cardInt2 >> cardInt3 >> cardInt4 >> cardInt5;

						bool areSameColor = false;	//check to see if all cities are same color
						bool areRepeatInput = true; //check to make sure same card wasn't inputted twice
						if (
							newMap.accessCity(cardInt1)->getColor == newMap.accessCity(cardInt2)->getColor &&
							newMap.accessCity(cardInt1)->getColor == newMap.accessCity(cardInt3)->getColor &&
							newMap.accessCity(cardInt1)->getColor == newMap.accessCity(cardInt4)->getColor &&
							newMap.accessCity(cardInt1)->getColor == newMap.accessCity(cardInt5)->getColor
							) {
							areSameColor = true;;
						}
						if (
							cardInt1 != cardInt2 && cardInt1 != cardInt3 && cardInt1 != cardInt4 && cardInt1 != cardInt5
							&& cardInt2 != cardInt3 && cardInt2 != cardInt4 && cardInt2 != cardInt5
							&& cardInt3 != cardInt4 && cardInt3 != cardInt5
							&& cardInt4 != cardInt5
							) {
							areRepeatInput = false;
						}


						if (areSameColor && !areRepeatInput) {\
							//TODO CHANGE CURE MARKER
							players.at(i).treatDisease(newMap); //DO WE CURE ON SAME TURN???

							players.at(i).discard(cardInt1);
							players.at(i).discard(cardInt2);
							players.at(i).discard(cardInt3);
							players.at(i).discard(cardInt4);
							players.at(i).discard(cardInt5);
						}
						else if (areRepeatInput) {
							cout << "You inputted the same index more than once..." << endl;
						}
						else if (!areSameColor) {
							cout << "You must input 5 cards of the same color" << endl;
						}
					}
					else {
						cout << "You must be at a research station to discover a cure" << endl;
					}
					break;
				}
				case '9':	//event cards
				{
					/*
					if (players.at(i).getHand()[cardInt]->getType() == "Event Card")
					players.at(i).displayHand();

					int cardInt = 0;
					while (cardInt == 0 || cardInt >= players.at(i).getHandSize()) {
						cin >> cardInt;
					}

					players.at(i).flight(players.at(i).getHand()[cardInt]->getId());

					players.at(i).discard(cardInt);
					break;
					*/
				}
				}
			}




			////////////END TURN / DRAW 2 PLAYER CARDS////////////
			players.at(i).concludeTurn(playerDeck);

			//players.at(i).savePlayer();
			//newMap.saveMap();



			/////////////INFECT CITIES///////////////
			cout << "------------------INFECTION TURN------------------" << endl;
			cout << "Cubes remaining: " << InfectionDeck::infectionCubes << ", Outbreaks: " << InfectionDeck::numberOutbreaks 
				<< ", Marker position: " << InfectionDeck::infectionMarker << "(" << InfectionDeck::markerValues[InfectionDeck::infectionMarker] << ")" << endl;
			ideck.playInfection();
			if (InfectionDeck::infectionCubes < 0) {
				//gameover = true;
				cout << "GAME OVER: You have run out of infection cubes" << endl;
				break;
			}
			if (InfectionDeck::numberOutbreaks >= 8) {
				//gameover = true;
				cout << "GAME OVER: Too many outbreaks have occurred" << endl;
				break;
			}
		}


	}


	system("pause"); //windows only
	return 0;
}
