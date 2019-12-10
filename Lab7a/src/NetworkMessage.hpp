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
			ServerGameState
		};


	public:
		NetworkMessage(Type type, std::string data) :
			_type(type),
			_data(data)
		{}


		Type GetType() const
		{
			return _type;
		}

		std::string GetData() const
		{
			return _data;
		}


	private:
		Type _type;
		std::string _data;
	};
}