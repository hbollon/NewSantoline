#include "stdafx.h"
#include "MessageContour.h"

#include <iostream>

MessageContour::MessageContour()
{
	m_message = MessageType::CONTOUR;
}

MessageContour::~MessageContour()
{
}

void MessageContour::handleMessage(nlohmann::json message)
{
	m_contour_initial.clear();
	for (nlohmann::json::iterator it = message["contour"].begin(); it != message["contour"].end(); ++it)
	{
		m_contour_initial.push_back(cleaned((*it)[0], (*it)[1]));
	}
}

Point2D cleaned(double x, double y)
{
	if (fmod(x, 25) < fmod(y, 25))
	{
		if (fmod(x, 25) < 12.5)
		{
			return Point2D(int(x - fmod(x, 25)), y);
		}
		else
		{
			return Point2D(int(x + fmod(x, 25)), y);
		}
	}
	else
	{
		if (fmod(y, 25) < 12.5)
		{
			return Point2D(x, int(y - fmod(y, 25)));
		}
		else
		{
			return Point2D(x, int(y + fmod(y, 25)));
		}
	}
}

std::vector<Point2D> MessageContour::contour_initial()
{
	return m_contour_initial;
}