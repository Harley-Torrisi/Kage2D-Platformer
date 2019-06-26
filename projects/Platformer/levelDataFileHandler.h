#pragma once
#include "level.h"

class levelDataFileHandler
{
public:
	levelDataFileHandler();
	~levelDataFileHandler();
	void SaveLevel(Level level, std::string path, std::string atlas, std::string background);
	Level LoadLevel(std::string path);
};

