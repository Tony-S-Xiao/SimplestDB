#include"simplestdb_util.h"

std::string sdb::serializeStringVec(const std::vector<std::string>& string_arr) {
  std::string serialized_string{ "" };
  for (int i = 0; i < string_arr.size(); ++i) {
    int curr_string_size = string_arr[i].size();
    /* Push placeholder characters( bytes ) */
    for (int j = 0; j < sizeof(int); ++j) {
      serialized_string.push_back(' ');
    }
    /* Copy the integer into string */
    typename std::string::iterator write_pos_iter = serialized_string.end();
    write_pos_iter -= sizeof(int);
    memcpy(&*write_pos_iter, &curr_string_size, sizeof(int));
  }
  return serialized_string;
}

std::vector<std::string> sdb::deserializeFormattedString(const std::string& input_string) {

}