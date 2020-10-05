#include "stdafx.h"
#include "PipeReader.h"


PipeReader::PipeReader(Tube::TubeMultiplexe* tube) : Reader(tube)
{
}


PipeReader::~PipeReader()
{
}

std::string PipeReader::read() 
{
	return m_tube->read_str();
}