#pragma once


namespace TTT
{
	class NetworkMessage
	{
	public:
		enum class Type
		{
			ClientConnectionAsk,
			ClientMMAsk,
			ClientGameAction,
			ClientDisconnect,

			ServerMMResult,
			ServerGameState,
			ServerGameResult
		};


	public:
		NetworkMessage(Type type, std::string data);


		Type GetType() const;

		std::string GetData() const;


		NetworkMessage(char* bytes, int bytesSize);

		char* GetMessageBytes(int* bytesSize) const;


	private:
		Type _type;
		std::string _data;
	};
}
