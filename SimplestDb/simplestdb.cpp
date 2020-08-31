#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>

using namespace std;

enum Command{INSERT=1, QUERY, EXIT, NONE};

class Parser
{

public:
	Parser() : data("")
	{

	}

	Parser(string command)
	{
		int endSpace = command.find_first_of(" ");
		data = command.substr(endSpace+1);

		auto currEle = lookupCommandTable.find(command.substr(0, endSpace));

		if (currEle != lookupCommandTable.end())
			commandType = lookupCommandTable.find(command.substr(0, endSpace))->second;
		else
			commandType = NONE;


	}

	~Parser()
	{

	}

	Command getCommand()
	{
		return commandType;
	}

	bool exit()
	{
		return commandType == EXIT ? true : false;
	}

protected:

private:

	string data;
	Command commandType;
	static unordered_map<string, Command> lookupCommandTable;

};

unordered_map<string, Command> Parser::lookupCommandTable = { {"insert", INSERT}, {".exit", EXIT}, {"select", QUERY} };


int main()
{

	string commandInput = "";

	cout << "Enter Command: ";
	getline(cin, commandInput);
	Parser currCommand = Parser(commandInput);
	
	while(true)
	{
		cout << currCommand.getCommand();
		cout << "Enter Command: ";
		getline(cin, commandInput);
		currCommand = Parser(commandInput);
	}

	return 0;
}

