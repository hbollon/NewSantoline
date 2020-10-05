#pragma once
#include "Message.h"
class MessageErreur : public Message
{
public:
	MessageErreur();
	~MessageErreur();
	virtual void handleMessage(nlohmann::json message);
};

