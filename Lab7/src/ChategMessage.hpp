#pragma once



class ChategMessage
{
public:
	ChategMessage(std::string text);


	static ChategMessage* Create(char* messageBytes, int bytesSize);

	char* GetMessageBytes(int* bytesSize) const;


	std::string ToString() const;


private:
	std::string _text;
};
