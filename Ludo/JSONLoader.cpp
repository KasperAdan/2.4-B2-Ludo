#include "JSONLoader.h"
#include <json/value.h>
#include <json/json.h>
#include <iostream>
#include <fstream>

using namespace std;

JSONLoader::JSONLoader() {
	
}

JSONLoader::~JSONLoader() {

}

bool JSONLoader::loadPositions(boardPositions* positions)
{
	ifstream file("Resource/Ludo_Positions.json");
	string jsonString;
	string newJsonString;
	string errors;

	// Loop through all the lines in Ludo_Positions and store each line into newJsonString
	while (getline(file, jsonString))
	{
		newJsonString = newJsonString + jsonString;
	}

	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	Json::Value root;

	// Parse newJsonString into root
	bool parsingSuccesful = reader->parse(newJsonString.c_str(), newJsonString.c_str() + newJsonString.size(), &root, &errors);
	delete reader;

	if (!parsingSuccesful) {
		cout << jsonString << endl;
		cout << errors << endl;
	}

	fillArrays(positions, root);
	

	return parsingSuccesful;
}

void JSONLoader::fillArrays(boardPositions* positions, Json::Value root) {
	Json::Value playArray = root["Play"];
	for (int i = 0; i < playArray.size(); i++)
		positions->playPositions[i] = glm::vec3(playArray[i][0].asFloat(), 0, playArray[i][1].asFloat());

	Json::Value blueStartArray = root["Blue"]["Start"];
	for (int i = 0; i < blueStartArray.size(); i++)
		positions->blueStartPositions[i] = glm::vec3(blueStartArray[i][0].asFloat(), 0, blueStartArray[i][1].asFloat());

	Json::Value blueEndArray = root["Blue"]["End"];
	for (int i = 0; i < blueEndArray.size(); i++)
		positions->blueEndPositions[i] = glm::vec3(blueEndArray[i][0].asFloat(), 0, blueEndArray[i][1].asFloat());
	
	Json::Value redStartArray = root["Red"]["Start"];
	for (int i = 0; i < redStartArray.size(); i++)
		positions->redStartPositions[i] = glm::vec3(redStartArray[i][0].asFloat(), 0, redStartArray[i][1].asFloat());

	Json::Value redEndArray = root["Red"]["End"];
	for (int i = 0; i < redEndArray.size(); i++)
		positions->redEndPositions[i] = glm::vec3(redEndArray[i][0].asFloat(), 0, redEndArray[i][1].asFloat());

	Json::Value greenStartArray = root["Green"]["Start"];
	for (int i = 0; i < greenStartArray.size(); i++)
		positions->greenStartPositions[i] = glm::vec3(greenStartArray[i][0].asFloat(), 0, greenStartArray[i][1].asFloat());

	Json::Value greenEndArray = root["Green"]["End"];
	for (int i = 0; i < greenEndArray.size(); i++)
		positions->greenEndPositions[i] = glm::vec3(greenEndArray[i][0].asFloat(), 0, greenEndArray[i][1].asFloat());

	Json::Value yellowStartArray = root["Yellow"]["Start"];
	for (int i = 0; i < yellowStartArray.size(); i++)
		positions->yellowStartPositions[i] = glm::vec3(yellowStartArray[i][0].asFloat(), 0, yellowStartArray[i][1].asFloat());

	Json::Value yellowEndArray = root["Yellow"]["End"];
	for (int i = 0; i < yellowEndArray.size(); i++)
		positions->yellowEndPositions[i] = glm::vec3(yellowEndArray[i][0].asFloat(), 0, yellowEndArray[i][1].asFloat());
}
