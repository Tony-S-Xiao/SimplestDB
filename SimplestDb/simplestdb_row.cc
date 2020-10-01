#pragma once
//VERSION 1

#include<utility>
#include<string>
#include<iostream>

sdb::Row::Row(uint32_t id, std::vector<SQLTypes> header) : id(id) {
	for (int i = 0; i < header.size(); ++i) {
		switch (header[i]) {
		  case SQLTypes::INTEGER: {
			integer.push_back(NULL);
			Row::header.insert({ i, std::make_pair(SQLTypes::INTEGER, integer.size() - 1) });
			break;
		  }
		  case SQLTypes::BOOLEAN: {
			boolean.push_back(NULL);
			Row::header.insert({ i, std::make_pair(SQLTypes::BOOLEAN, boolean.size() - 1) });
			break;
		  }
		  case SQLTypes::BLOB: {
			blob.push_back(NULL);
			Row::header.insert({ i, std::make_pair(SQLTypes::BLOB, blob.size() - 1) });
			break;
		  }
		  case SQLTypes::VARCHAR: {
			varchar.push_back("");
			Row::header.insert({ i, std::make_pair(SQLTypes::VARCHAR, varchar.size() - 1) });
			break;
		  }
		}
	}
}

//TODO: deserialization
sdb::Row::Row(std::vector<SQLTypes> header, std::vector<unsigned char> data) {
	//TODO
}

sdb::Row::~Row() {
	//nothing to delete
}

//TODO: check if there is a better way to use these templates
//
template<typename A>
A sdb::Row::getField(int i) const {
	return NULL;
}
template<> int sdb::Row::getField<int>(int i) const {
	if (i < 0 || i >= header.size())
		return 0;
	std::unordered_map<int, std::pair<SQLTypes, int>>::const_iterator curr = header.find(i);
	return integer[curr->second.second];
}
template<> unsigned char sdb::Row::getField<unsigned char>(int i) const {
	if (i < 0 || i >= header.size())
		return 0;
	std::unordered_map<int, std::pair<SQLTypes, int>>::const_iterator curr = header.find(i);
	return blob[curr->second.second];
}
template<> bool sdb::Row::getField<bool>(int i) const {
	if (i < 0 || i >= header.size())
		return 0;
	std::unordered_map<int, std::pair<SQLTypes, int>>::const_iterator curr = header.find(i);
	return boolean[curr->second.second];
}
template<> std::string sdb::Row::getField<std::string>(int i) const {
	if (i < 0 || i >= header.size())
		return 0;
	std::unordered_map<int, std::pair<SQLTypes, int>>::const_iterator curr = header.find(i);
	return varchar[curr->second.second];
}

template<typename T> 
void sdb::Row::setField(int i, T data) {
	return;
}
template<> void sdb::Row::setField<int>(int i, int data) {
	if (i < 0 || i >= header.size())
		return;
	std::unordered_map<int, std::pair<SQLTypes, int>>::iterator curr = header.find(i);
	integer[curr->second.second] = data;
	return;
}
template<> void sdb::Row::setField<unsigned char>(int i, unsigned char data) {
	if (i < 0 || i >= header.size())
		return;
	std::unordered_map<int, std::pair<SQLTypes, int>>::iterator curr = header.find(i);
	blob[curr->second.second] = data;
	return;
}
template<> void sdb::Row::setField<bool>(int i, bool data) {
	if (i < 0 || i >= header.size())
		return;
	std::unordered_map<int, std::pair<SQLTypes, int>>::iterator curr = header.find(i);
	boolean[curr->second.second] = data;
	return;
}
template<> void sdb::Row::setField<std::string>(int i, std::string data) {
	if (i < 0 || i >= header.size())
		return;
	std::unordered_map<int, std::pair<SQLTypes, int>>::iterator curr = header.find(i);
	varchar[curr->second.second] = data;
	return;
}

uint32_t sdb::Row::getID() const {
	return id;
}

//TODO: serialization into a page
std::vector<unsigned char> sdb::Row::serialize() const {
	return {};
}

//TODO: better text justification for long rows
void sdb::Row::printRow() const {
	auto curr = header.begin();
	while (curr != header.end()) {
		switch (curr->second.first) {
		  case SQLTypes::INTEGER: {
			  std::cout << integer[curr->second.second] << " | ";
			break;
		  }
		  case SQLTypes::BOOLEAN: {
			  std::cout << boolean[curr->second.second] << " | ";
			break;
		  }
		  case SQLTypes::BLOB: {
			  std::cout << blob[curr->second.second] << " | ";
			break;
		  }
		  case SQLTypes::VARCHAR: {
			  std::cout << varchar[curr->second.second] << " | ";
			break;
		  }
		}
		++curr;
	}
	std::cout << std::endl;
	return;
}