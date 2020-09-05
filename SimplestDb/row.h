#pragma once
#include<vector>
#include<string>
#include<bitset>

using namespace std;

class Row
{
public:
	Row();
	~Row();

private:

	uint32_t id;
	vector<uint32_t> offsets;
	vector<int> integer;
	vector<bool> boolean;
	vector<bool> blob; //need dynamic bitset!!
	vector<string> varchar;

};
