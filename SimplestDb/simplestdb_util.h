#pragma once
#include<string>
#include<vector>
#include<array>

namespace sdb {

  /* Serializes an array of strings. 
  ** The first 4 bytes of the output string is a 32-bit interger denoting size in
  ** bytes of the string that follows. Then another 4 bytes denotes the size of the
  ** next string and so on.
  */
  std::string serializeStringVec(const std::vector<std::string>& string_arr);
  /* Deserializes the string into a vector of strings.
  ** Input must follow the format defined by the function serializeStringVec.
  ** Behaviour is undefined for incorrectly formatted inputs.
  */
  std::vector<std::string> deserializeFormattedString(std::string input_string);
}