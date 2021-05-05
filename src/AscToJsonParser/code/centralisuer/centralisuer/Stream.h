#pragma once
#include "pipe.h"

class Stream
{
public:
	Stream(Tube::TubeMultiplexe *tube);
	~Stream();
	virtual bool connected();

protected:
	Tube::TubeMultiplexe *m_tube;
};
