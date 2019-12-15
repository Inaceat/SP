#pragma once


namespace TTT
{
	class UserCommand
	{
	public:
		enum class Type
		{
			FindGame,
			DoGameAction,
			Exit
		};


	public:
		UserCommand(Type type, std::string data);


		Type GetType() const;

		std::string GetData() const;


	private:
		Type _type;
		std::string _data;
	};	
}
