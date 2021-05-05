#include "stdafx.h"
#include "Stream.h"

Stream::Stream(Tube::TubeMultiplexe *tube) : m_tube(tube)
{
	m_tube->connect();
}

Stream::~Stream()
{
}

bool Stream::connected()
{
	return m_tube->is_connected();
}
