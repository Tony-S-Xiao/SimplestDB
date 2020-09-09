//#include"parser.h"
//#include"token.h"
#include"simplestdb_row.h"

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>

int main()
{
	sdb::Row wow(12, { SQLTypes::BLOB, SQLTypes::BLOB, SQLTypes::INTEGER, SQLTypes::VARCHAR, SQLTypes::BOOLEAN });

	//wow.setField<unsigned char>(0, 'X');
	//wow.setField<unsigned char>(1, 'X');
	wow.setField<int>(2, 100);
	//wow.setField<std::string>(3, std::string("wow string."));
	//wow.setField<bool>(4, false);
	wow.printRow();
	//while (true)
	//{
	//	string commandInput = "";
	//	printPrompt("");
	//	cout << "Enter Command: ";
	//	getline(cin, commandInput);
	//	//Parser p;

	//	//Token* t = p.initToken(commandInput);
	//	
	//	if (t == nullptr) continue;
	//	cout << t->getText() << endl;
	//}
	return 0;
}

//string printPrompt(string toPrint)
//{
//	static unordered_map<string, string> promptList;
//
//		string line;
//		ifstream myfile("config.ini");
//
//		cout << myfile.is_open() << endl;
//		if (myfile.is_open())
//		{
//			while (myfile.good())
//			{
//				getline(myfile, line);
//				cout << line << "\n";
//			}
//		}
//		myfile.close();
//
//	return "";
//}

