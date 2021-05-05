#pragma once
#include <string>
#include "pipe.h"
#include "Stream.h"

class Reader : public Stream
{
public:
	Reader(Tube::TubeMultiplexe *tube);
	~Reader();
	virtual std::string read() = 0;
	virtual void write(std::string message) = 0;
};
