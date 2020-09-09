// rows should:
// constant time get/set to any field given offset
// serialize/deserialize itself 
// initialize to the correct capacity
// rows should not:
// hold column order
//
// 
#pragma once
#include<vector>
#include<string>
#include<utility>
#include<unordered_map>

enum SQLTypes { INTEGER, BOOLEAN, BLOB, VARCHAR };

namespace sdb {

class Row {
 public:
  Row(uint32_t id, std::vector<SQLTypes> header);
  Row(std::vector<SQLTypes> header, std::vector<unsigned char> data);
  ~Row();

  uint32_t getID() const;
  // i is the overall index in the table. no type check is done
  template<typename A>
  A getField(int i) const;

  template<typename T>
  void setField(int i, T data) {
	  if (i < 0 || i >= header.size())
		  return;

	  auto curr = header.find(i);
	  switch (curr->first) {
	  case INTEGER: {
		  integer[curr->second] = data;
		  break;
	  }
	  case BOOLEAN: {
		  boolean[curr->second] = data;
		  break;
	  }
	  case BLOB: {
		  blob[curr->second] = data;
		  break;
	  }
	  case VARCHAR: {
		  varchar[curr->second] = data;
		  break;
	  }
	  }
	  return;
  }


  std::vector<unsigned char> serialize() const;
  void printRow() const;
  
 private:
  uint32_t id;
  std::unordered_map<int, std::pair<SQLTypes, int>> header;
  std::vector<int> integer;
  std::vector<bool> boolean;
  std::vector<std::string> varchar;
  std::vector<unsigned char> blob;
};

} //namespace sdb