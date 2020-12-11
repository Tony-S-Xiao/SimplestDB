#ifndef SIMPLESTDB_PAGE_FOOTER_H_
#define SIMPLESTDB_PAGE_FOOTER_H_

#include<cstddef>
#include<cstdint>

namespace sdb {

class Footer {
 public:
  // Constructs the footer on the page using the address of the array.end().
  Footer(std::byte*);
  uint16_t& operator[](uint16_t index);
  // Removes the last element.
  // Pop back on an empty array is throws out-of-range.
  void pop_back();
  // Appends data to the last element.
  void push_back(uint16_t data);
  uint16_t size();
  // Gets the total size(elements + metadata) of the footer in bytes.
  uint16_t physicalSize();
  // Gets the pointer to the freespace location on page.
  uint16_t getFreeSpacePtr();
  // Sets the pointer to the freespace location on page.
  void setFreeSpace(uint16_t data);
 private:
  // Points to the front of the on-page vector.
  unsigned short* start_of_arr{ nullptr };
  // Points to the back of the on-page vector.
  unsigned short* top_ptr{ nullptr };
  // Holds the current free space address.
  unsigned short* free_space_ptr{ nullptr };
  // Holds the current number of elements in the footer.
  unsigned short* size_ptr{ nullptr };
};
}  // namespace sdb
#endif // !SIMPLESTDB_PAGE_FOOTER_H_