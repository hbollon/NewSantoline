#pragma once
#include "Message.h"
#include "MessageAlgorithme.h"
#include "MessageContour.h"
#include "MessageEpilobe.h"

#include "Reader.h"

class MessageSimulation : public Message
{
public:
	MessageSimulation();
	~MessageSimulation();
	virtual void handleMessage(nlohmann::json message);
	std::vector<Point3D> execute();

	void set_algorithm(MessageAlgorithme* algorithme);
	void set_epilobe(MessageEpilobe* epilobe);
	void set_contour(MessageContour* contour);
	void set_reader(Reader* m_reader);

	nlohmann::json vectorPointToJSON(std::vector<Point3D> points);
private:
	MessageAlgorithme* m_algorithme;
	MessageEpilobe* m_epilobe;
	MessageContour* m_contour;
	Reader* m_reader;
};

