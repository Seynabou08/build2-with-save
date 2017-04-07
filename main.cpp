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
	// Initializing the map 
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

	// Setting up the player deck and the player discard pile
	vector<Card*> playerDeck;


	for (int i = 0; i < newMap.cities.size(); i++) {
		CityCard* ccard = new CityCard(newMap.cities[i]);
		Card* card = new Card();
		card = ccard;

		playerDeck.push_back(card);
	}

	//We need a discard Pile of event card that the contengency planner can pick from.
	vector<Card*> discardPile;

	//Generating event card to add to player deck
	string eventNames[] = { "Airlift", "One Quiet Night", "Resilient population",
		"Government Grant", "Forcast" };

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

	string roleNames[] = { "Contingency Planner", "Dispatcher", "Medic",
		"Operation Expert", "Quarantine Specialist", "Researcher", "Scientist" };

	Role* roleCard = new Role();
	for (int i = 0; i < 7; i++)
	{
		roleCard->setRole(roleNames[i], i + 1);
		roleDeck.push_back(roleCard);
	}


	//Shuffling the Player cards randomly
	random_shuffle(playerDeck.begin(), playerDeck.end());
	random_shuffle(roleDeck.begin(), roleDeck.end());


	//Infection deck initialization
	cout << "INITIALIZING INFECTION DECK" << endl;
	InfectionDeck ideck = InfectionDeck(newMap.cities);
	ideck.init(48);
	ideck.initialInfection();


	for (int i = 0; i < playerNum; i++) {

		Player* player = &players.at(i);

		player->drawCards(newMap,playerDeck, cardNum);

		player->setRole(*roleDeck.back());

		roleDeck.pop_back();
	}


	//Displaying player locations
	for (int i = 0; i < playerNum; i++) cout << "Player " << i + 1 << " is at " << newMap.cities[players.at(i).getLocation()]->getName() << endl;


	bool gameover = false;

	// This loop ensures that each player gets a turn one after te other followed by infection turns
	while (!gameover) {   //If gameover is true we will exit the loop and that ends the game

		for (int i = 0; i < playerNum; i++) {
			players.at(i).startTurn();
			cout << "------------------PLAYER " << i + 1 << " TURN------------------" << endl;
			while (players.at(i).checkAction())
			{
				//Displaying the possible actions the player can make
				cout << "Player " << i + 1 << " is at " << newMap.cities[players.at(i).getLocation()]->getName() 
					<< ". That player is a " << players.at(i).getRole() << "." << endl;
				cout << "Player " << i + 1 << ": Choose your action..." << endl;

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
				if (players.at(i).getRole() == "Contingency Planner" || players.at(i).getRole() == "Operation Expert")
					cout << "0. Role" << endl;


				char action;
				cin >> action;

				switch (action) {
				case '1':	//Drive/Ferry
				{
					int choice = i;

					if (players.at(i).getRole() == "Contingency Planner")
					{
						cout << "Which player's pawn do you want to move?";
						cin >> choice;
						while (choice <= 0 || choice > playerNum) cin >> choice;

					}

					newMap.showCity(players.at(choice).getLocation());
					players.at(choice).move(newMap);

					break;
				}
				case '2':	//Direct Flight: Discard a City card to move to the city named on the card.
				{

					int choice = i;

					if (players.at(i).getRole() == "Dispatcher")
					{
						cout << "Which player's pawn do you want to move?";
						cin >> choice;
						while (choice <= 0 || choice > playerNum) cin >> choice;

					}

					players.at(i).displayHand();

					if (players.at(i).getHandSize() == 0)
					{
						cout << "You are out of city cards select another action." << endl;
						break;
					}
					cout << "Which city do you want to move to" << endl;

					int cardInt = -1;
					while (cardInt == -1 || cardInt >= players.at(i).getHandSize()) {
						cin >> cardInt;
					}

					players.at(choice).flight(players.at(i).getHand()[cardInt]->getId());

					players.at(i).discard(cardInt);
					break;
				}
				case '3':	//Charter Flight: Discard the City card that matches the city you are in to move to any city.
				{
					int choice = i;

					if (players.at(i).getRole() == "Dispatcher")
					{
						cout << "Which player's pawn do you want to move?";
						cin >> choice;
						while (choice <= 0 || choice > playerNum) cin >> choice;

					}

					//check for matching card
					bool hasMatchingCard = false;
					int matchingCardIndex;
					for (int j = 0; j < players.at(i).getHandSize(); j++) {
						if (players.at(i).getHand()[j]->getId() == players.at(choice).getLocation()) {
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
						players.at(choice).flight(cityID);
						players.at(i).discard(matchingCardIndex);
					}
					else {
						cout << "You cannot take a charter flight as you do not have a card matching the city you are currently on." << endl;
					}

					break;
				}
				case '4': //Shuttle Flight	Move from a city with a research station to any other city that has a research station.
				{

					int choice = i;

					if (players.at(i).getRole() == "Dispatcher")
					{
						cout << "Which player's pawn do you want to move?";
						cin >> choice;
						while (choice <= 0 || choice > playerNum) cin >> choice;

					}


					City* location = newMap.accessCity(players.at(choice).getLocation());
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
								players.at(choice).flight(newLocation);
								break;
							}
						}
					}
					break;
				}
				case '5': //Building a Research Station

				{
					players.at(i).buildStation(&newMap);
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
					cout << "With which player do you want to share knowledge?" << endl;
					int a = i;
					while (a >= players.size() || a == i) {
						cin >> a;
						if (a == i) {
							cout << "You cannot share knowledge with yourself...try again" << endl;

						}
						if (a >= players.size()) {
							cout << "There are only " << players.size() << " players...try again" << endl;

						}

					}
					Player* buddy = &players.at(a);
					players.at(i).shareKnowledge(buddy);
					break;
				}

				case '8'://Discover a Cure
				{
					players.at(i).discoverCure(newMap);
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

				case '0':
				{
					if (players.at(i).getRole() == "Dispatcher")
					{
						int choice = i;
						int location;
						City* newLoc;

						cout << "Which player's pawn do you want to move?" << endl;
						cin >> choice;
						while (choice <= 0 || choice > playerNum) cin >> choice;

						cout << "Which city will you move it to ?";
						for (int k = 0; k < playerNum; k++)
						{
							newLoc = newMap.accessCity(players.at(k).getLocation());
							cout << newLoc->index << " : " << newLoc->getName() << endl;
						}

						newMap.showCity(players.at(choice).getLocation());
						players.at(choice).move(newMap);

					}

					if (players.at(i).getRole() == "Operations Expert")
					{
						players.at(i).displayHand();
						cout << "Which city do you want to move to?";

						int cardInt = -1;
						while (cardInt == -1 || cardInt >= players.at(i).getHandSize()) {
							cin >> cardInt;
						}

						players.at(i).flight(players.at(i).getHand()[cardInt]->getId());

						players.at(i).discard(cardInt);
						break;
					}

					else
					{
						cout << "You are neither a dispatcher nor a dispatcher you can't do this!";
						break;
					}

					break;

				}
				}
			}




			////////////END TURN / DRAW 2 PLAYER CARDS////////////
			players.at(i).concludeTurn(playerDeck, newMap);

			players.at(i).savePlayer();
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