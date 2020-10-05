#pragma once
#include "Reader.h"
#include "pipe.h"

class PipeReader : public Reader
{
public:
	PipeReader(Tube::TubeMultiplexe* tube);
	~PipeReader();
	virtual std::string read();
};

