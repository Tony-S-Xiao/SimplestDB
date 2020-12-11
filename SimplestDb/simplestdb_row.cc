#include"simplestdb_row.h"

#include<cstddef>

sdb::Row::Row(std::byte* begin, std::byte* end) :
  row_begin_(begin), row_end_(end) {
}
size_t sdb::Row::physicalSize() {
  return row_end_ - row_begin_;
}
