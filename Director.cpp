#include "MapSaver.h"
#include "PlayerSaver.h"
#include <iostream>
#include <fstream>
class Director
{
private:
	AbstractBuilder *builder;

public:

	void setBuilder(AbstractBuilder *builder) { this->builder = builder; };

	ifstream myfile;
	myfile.open("save.txt", ifstream::in);


	void ConstructMap(MapSaver* mapSaver )
	{
		mapSaver->Load(myfile);
	}
	void ConstructPlayer(PlayerSaver* playerSaver)
	{
		playerSaver->Load(myfile);
	}

	void SaveMap(MapSaver* mapSaver)
	{
		mapSaver->Save();
	}
	void SavePlayer(PlayerSaver* playerSaver)
	{
		playerSaver->Save();
	}

};
