#ifndef SIMPLESTDB_ROW_H_
#define SIMPLESTDB_ROW_H_

#include<cstddef>
#include<cstdint>
// These objects interpret data blocks on Slottedpage objects using pointers.
// All data reside on an array of unsigned chars(see Slottedpage class).
// These can act both like records in tables or on-disk-pointers used in 
// .sdb files or metadata for the database.
namespace sdb {
// Booleans are 1 byte to avoid vector<bool> specialization.
using bool8_t = char;
// Pointers in the rows are unsigned shorts(in number of bytes) offset from where the pointer starts.
// Rows cannot span more than 1 page. So max pointer value only needs to be 2^16. Same as page size.
using OnRowPointer = uint16_t;
// On disk pointers(page id) are the index of page in .sdb file 0th indexed.
using OnDiskPointer = uint32_t;
// Base class to be derived.
class Row {
 public:
  // Number of bytes of the row.
  size_t physicalSize();
 protected:
  // Only used by derived classes. Requires [begin, end).
  Row(std::byte* begin, std::byte* end);
  std::byte* row_begin_;
  std::byte* row_end_;
};
}  // namespace sdb
#endif // !SIMPLESTDB_ROW_H_