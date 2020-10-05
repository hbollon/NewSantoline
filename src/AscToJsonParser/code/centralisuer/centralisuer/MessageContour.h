#pragma once
#include "Message.h"

#include "useful.h"
class MessageContour : public Message
{
public:
	MessageContour();
	~MessageContour();
	virtual void handleMessage(nlohmann::json message);

	std::vector<Point2D>contour_initial();
protected:
	std::vector<Point2D> m_contour_initial;
};

Point2D cleaned(double x, double y);
