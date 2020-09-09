#pragma once
#include"simplestdb_row.h"

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

sdb::Row::Row(std::vector<SQLTypes> header, std::vector<unsigned char> data) {
	//TODO
}

sdb::Row::~Row() {
	//nothing to delete
}

template<typename A>
A sdb::Row::getField(int i) const {
	if (i < 0 || i >= header.size())
		return typeid(A) == typeid(std::string) ? "" : NULL;

	auto curr = header.find(i);
	switch (curr->first) {
	  case SQLTypes::INTEGER: {
		return integer[curr->second];
	  }
	  case SQLTypes::BOOLEAN: {
		return boolean[curr->second];
	  }
	  case SQLTypes::BLOB: {
		return blob[curr->second];
	  }
	  case SQLTypes::VARCHAR: {
		return varchar[curr->second];
	  }
	}
	return typeid(A) == typeid(std::string) ? "" : NULL;
}

//template<typename T>
//void sdb::Row::setField(int i, T data) {
//	if (i < 0 || i >= header.size())
//		return;
//
//	auto curr = header.find(i);
//	switch (curr->first) {
//	  case SQLTypes::INTEGER: {
//		integer[curr->second] = data;
//		break;
//	  }
//	  case SQLTypes::BOOLEAN: {
//		boolean[curr->second] = data;
//		break;
//	  }
//	  case SQLTypes::BLOB: {
//		blob[curr->second] = data;
//		break;
//	  }
//	  case SQLTypes::VARCHAR: {
//		varchar[curr->second] = data;
//		break;
//	  }
//	}
//	return;
//}

uint32_t sdb::Row::getID() const {
	return id;
}

std::vector<unsigned char> sdb::Row::serialize() const {
	return {};
}

void sdb::Row::printRow() const {
	//auto curr = header.find(0);
	//for (int i = 0; i < header.size(); ++i) {
	//	
	//	switch (curr->first) {
	//	  case SQLTypes::INTEGER: {
	//		
	//		break;
	//	  }
	//	  case SQLTypes::BOOLEAN: {
	//		
	//		break;
	//	  }
	//	  case SQLTypes::BLOB: {
	//		
	//		break;
	//	  }
	//	  case SQLTypes::VARCHAR: {
	//		
	//		break;
	//	  }
	//	  default:
	//	}
	//}

	return;
}