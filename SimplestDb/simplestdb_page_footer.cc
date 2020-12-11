#include"simplestdb_page_footer.h"

#include<stdexcept>
#include<limits>
#include<cmath>
#include<cstddef>
#include<cstdint>
sdb::Footer::Footer(std::byte* end_of_arr) :
start_of_arr(reinterpret_cast<unsigned short*>(end_of_arr) - 3), 
top_ptr(reinterpret_cast<unsigned short*>(end_of_arr) - 3),
free_space_ptr(reinterpret_cast<unsigned short*>(end_of_arr) - 2),
size_ptr(reinterpret_cast<unsigned short*>(end_of_arr) - 1)
{
  if (*size_ptr > 0) {
    top_ptr -= *size_ptr;
  }
}
uint16_t& sdb::Footer::operator[](uint16_t index) {
  if(start_of_arr - index > top_ptr && start_of_arr - index < free_space_ptr)
  return *(start_of_arr - index);
  throw std::out_of_range("Index of Footer out of range.");
}
void sdb::Footer::pop_back() {
  if (size_ptr > 0) {
  ++top_ptr;
  --*size_ptr;
  return;
  }
  throw std::out_of_range("Pop back on empty footer.");
}
void sdb::Footer::push_back(uint16_t data) {
  *top_ptr = data;
  --top_ptr;
  ++*size_ptr;
}
uint16_t sdb::Footer::size() {
  return static_cast<size_t>(*size_ptr);
}
uint16_t sdb::Footer::physicalSize() {
  return abs(top_ptr - start_of_arr)*2 + 2 + 2;
}
uint16_t sdb::Footer::getFreeSpacePtr() {
  return *free_space_ptr;
}
void sdb::Footer::setFreeSpace(uint16_t data) {
  *free_space_ptr = data;
}