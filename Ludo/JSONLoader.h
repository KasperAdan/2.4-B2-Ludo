#pragma once
#include <glm/glm.hpp>
#include <json/value.h>
class JSONLoader
{
	 struct boardPositions;
private:
	void fillArrays(boardPositions*, Json::Value);

public:
	JSONLoader();
	~JSONLoader();
	bool loadPositions(boardPositions*);

   struct boardPositions
    {
		glm::vec3 playPositions[40];
		glm::vec3 blueStartPositions[4];
		glm::vec3 blueEndPositions[4];
		glm::vec3 redStartPositions[4];
		glm::vec3 redEndPositions[4];
		glm::vec3 greenStartPositions[4];
		glm::vec3 greenEndPositions[4];
		glm::vec3 yellowStartPositions[4];
		glm::vec3 yellowEndPositions[4];
    };
};


