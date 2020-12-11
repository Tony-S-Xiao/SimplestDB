#ifndef SIMPLESTDB_UTIL_H_
#define SIMPLESTDB_UTIL_H_

#include<string>
#include<vector>

namespace sdb {
  /* The first 4 bytes of the output string is a 32-bit interger denoting size in
  ** bytes of the string that follows. Then another 4 bytes denotes the size of the
  ** next string and so on. NULL terminated.
  */
  std::string serializeStringVec(const std::vector<std::string>& string_arr);
  /* Input must follow the format defined by the function serializeStringVec.
  ** Behaviour is undefined for incorrectly formatted inputs.
  */
  std::vector<std::string> deserializeFormattedString(const std::string& input_string);
}
#endif // !SIMPLESTDB_UTIL_H_