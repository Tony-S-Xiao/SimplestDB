#include"simplestdb_util.h"

#include<string>
#include<vector>

std::string sdb::serializeStringVec(const std::vector<std::string>& string_arr) {
  std::string serialized_string{ "" };
  for (int i = 0; i < string_arr.size(); ++i) {
    unsigned short curr_string_size = string_arr[i].size() + sizeof(unsigned short);
    /* Push 2 placeholder characters( bytes ) */
    for (int j = 0; j < sizeof(unsigned short); ++j) {
      serialized_string.push_back(' ');
    }
    /* Copy the pointer into the string */
    typename std::string::iterator write_pos_iter = serialized_string.end();
    write_pos_iter -= sizeof(unsigned short);
    memcpy(&*write_pos_iter, &curr_string_size, sizeof(unsigned short));
    serialized_string.append(string_arr[i]);
  }
  serialized_string.append({ 0,0 }); /* NULL terminated. TODO: do I need to be careful here? how does c++ treat null terminated string? */
  return serialized_string;
}

std::vector<std::string> sdb::deserializeFormattedString(const std::string& input_string) {
  int num_of_bytes{ 0 };
  std::vector<std::string> result{};
  typename std::string::const_iterator read_pos_iter = input_string.begin();

  while (read_pos_iter < input_string.end()) {
    memcpy(&num_of_bytes, &*read_pos_iter, sizeof(int));
    read_pos_iter += sizeof(int);
    result.push_back(input_string.substr(std::distance(read_pos_iter, input_string.begin()), num_of_bytes));
    read_pos_iter += num_of_bytes;
  }
  return result;
}