#pragma once
#include "Message.h"
class MessageActions : public Message
{
public:
	MessageActions();
	~MessageActions();
	void handleMessage(nlohmann::json message);
};

