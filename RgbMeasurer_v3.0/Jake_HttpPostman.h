#pragma once

#include <SPI.h>
#include <Phpoc.h>
#include "Jake_TCS34725.h"

class HttpPostman {
private:
	PhpocClient client;
	const char* server = "210.102.142.14";
	bool debug;
public:
  HttpPostman(void);
	void begin(bool debug);
	void sendLightChar(TCS34725&);
};
