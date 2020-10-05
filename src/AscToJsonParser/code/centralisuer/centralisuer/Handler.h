#pragma once
#include <algorithm>
#include <string>
#include <iostream>
#include "Reader.h"

#include "useful.h"

#include "MessageAlgorithme.h"
#include "MessageEpilobe.h"

class Handler
{
public:
	Handler(Reader* reader);
	~Handler();
	virtual void run();
private:
	bool m_running;
	Reader* m_reader;
};

