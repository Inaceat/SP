#pragma once


enum class MessageType
{
	Text,
	Service
};

//TODO make binary serialization using << >>
class ChategMessage
{
public:
	ChategMessage(MessageType type, std::string text);

	ChategMessage(std::string text);


	static ChategMessage* Create(char* messageBytes, int bytesSize);

	char* GetMessageBytes(int* bytesSize) const;


	std::string ToString() const;


private:
	MessageType _type;

	std::string _text;
};
