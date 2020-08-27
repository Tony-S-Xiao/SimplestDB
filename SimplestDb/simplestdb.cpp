#include<iostream>
#include<string>


using namespace std;

int main()
{
//loop to take in inputs

	string commandInput = "";
	string exit = ".exit"; //whats the best way to check for these commands?
	
	while (commandInput != exit)
	{

		cout << "Enter Command: " << endl;
		cin >> commandInput;
		cout << commandInput;
		

	}

	return 0;
}

class Parser
{

	public:
		Parser() : data("")
		{
			
		}
		Parser(string command)
		{
			data = command;
		}
		~Parser() 
		{

		}

	protected:

	private:

		string data;





};