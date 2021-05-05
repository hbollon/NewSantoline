#pragma once
#include "Reader.h"
#include "pipe.h"
#include <string>

class FileReader : public Reader
{
public:
	FileReader(Tube::TubeMultiplexe *tube);
	~FileReader();
	virtual std::string read();
	virtual void write(std::string message);

private:
	std::string m_filename;
};
