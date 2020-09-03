#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include"parser.h"
#include"token.h"
#include<fstream>

using namespace std;

string printPrompt(string);

int main()
{
	while (true)
	{
		string commandInput = "";
		printPrompt("");
		cout << "Enter Command: ";
		getline(cin, commandInput);
		Parser p;

		Token* t = p.initToken(commandInput);
		
		if (t == nullptr) continue;
		cout << t->getText() << endl;
	}



	return 0;
}

string printPrompt(string toPrint)
{
	static unordered_map<string, string> promptList;

		string line;
		ifstream myfile("config.ini");

		cout << myfile.is_open() << endl;
		if (myfile.is_open())
		{
			while (myfile.good())
			{
				getline(myfile, line);
				cout << line << "\n";
			}
		}
		myfile.close();

	return "";
}