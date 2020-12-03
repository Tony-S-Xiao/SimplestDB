#pragma once
#include"simplestdb_page_footer.h"

#include<utility>
#include<array>

/*
this is the main object responsible for the physical layout of the page
all data in the database should go on these pages
the page object allocates a block of bytes on the page
the footer object is used to keep track of the positions of the blocks

*/

namespace sdb {

//hard coded page size 2^16 bytes
constexpr size_t kPageSize{ 65536 };

class SlottedPage {
public:
		SlottedPage();
		//the array can moved into the data of the page
		SlottedPage(std::array<unsigned char, kPageSize>&);
		~SlottedPage();
		//gets the address of the array within the page
		//used to write the block of data into the disk
		void* getPageStart();
		//gets the block by slot id
		std::pair<void*, void*> getBlock(int page_id);
		//gets the size in bytes of the total space used on the page
		size_t physicalSize();
		//gets the size in bytes of free space on the page
		size_t freeSpace();
		//gets the size in number of slots used 
		size_t size();
		//returns start, end, slot id of allocatedBlock
		std::tuple<void*, void*, unsigned short> allocateBlock(size_t bytes_required);

		unsigned int getNextPage() const;
		unsigned int getPrevPage() const;
		void setNextPage(unsigned int index);
		void setPrevPage(unsigned int index);
private:
		//helper function used to get the slot id of the first 
		int getEmptyFooterSlot();
		//arbitraily gets the address of one byte on the page. unused
		void* getAddressOfElement(unsigned short index);
		//array of bytes(unsigned char)
		std::array<unsigned char, kPageSize> data{};
		//footer used to keep track of the location of blocks
		//see footer class
		Footer* footer;
		//a disk pointer to the index of next page
		unsigned int* next_page;
		//a disk pointer to the index of prev page
		unsigned int* prev_page;
};

} // namespace sdb