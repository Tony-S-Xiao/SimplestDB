#pragma once

namespace sdb {

//an on-page vector

class Footer {
public:
  Footer();
  //constructs the footer on the page using the address of the end of the array + 1
  Footer(void*);
  ~Footer();
  //access by index
  unsigned short& operator[](size_t index);
  //removes the last element
  //pop_back on an empty array is undefined
  void pop_back();
  //appends data to the last element
  void push_back(unsigned short data);
  //gets the total number of elements in the array
  size_t size();
  //gets the total size(elements + metadata) of the footer in bytes
  size_t physicalSize();
  //gets the pointer to the freespace location on page
  unsigned short getFreeSpacePtr();
  //sets the pointer to the freespace location on page
  void setFreeSpace(unsigned short data);
private:
  //points to the front of the on-page vector
  unsigned short* start_of_arr{ nullptr };
  //points to the back of the on-page vector
  unsigned short* top_ptr{ nullptr };
  //holds the current free space address
  unsigned short* free_space_ptr{ nullptr };
  //holds the current number of elements in the footer
  unsigned short* size_ptr{ nullptr };
};

}