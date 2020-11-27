#include"simplestdb_page_footer.h"

#include<stdexcept>
#include<limits>
#include<cmath>

sdb::Footer::Footer() {
}
sdb::Footer::Footer(void* end_of_arr) :
start_of_arr(static_cast<unsigned short*>(end_of_arr) - 3), 
top_ptr(static_cast<unsigned short*>(end_of_arr) - 3), 
free_space_ptr(static_cast<unsigned short*>(end_of_arr) - 2),
size_ptr(static_cast<unsigned short*>(end_of_arr) - 1)
{
  if (*size_ptr > 0) {
    top_ptr -= *size_ptr;
  }
}
sdb::Footer::~Footer() {

}
unsigned short& sdb::Footer::operator[](size_t index) {
  if(start_of_arr - index > top_ptr && start_of_arr - index < free_space_ptr)
  return *(start_of_arr - index);
  throw std::out_of_range("Index of Footer out of range.");
}
void sdb::Footer::pop_back() {
  ++top_ptr;
  --*size_ptr;
}
void sdb::Footer::push_back(unsigned short data) {
  *top_ptr = data;
  --top_ptr;
  ++*size_ptr;
}
size_t sdb::Footer::size() {
  return static_cast<size_t>(*size_ptr);
}
size_t sdb::Footer::physicalSize() {
  return abs(top_ptr - start_of_arr)*2 + 2 + 2;
}
unsigned short sdb::Footer::getFreeSpacePtr() {
  return *free_space_ptr;
}
void sdb::Footer::setFreeSpace(unsigned short data) {
  *free_space_ptr = data;
}
