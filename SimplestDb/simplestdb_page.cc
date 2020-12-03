#include"simplestdb_page.h"

#include<vector>
#include<algorithm>
#include<iostream>
#include<functional>

sdb::SlottedPage::SlottedPage() {
		footer = new Footer(static_cast<void*>(&data[65535] + 1 - 8));
		prev_page = reinterpret_cast<unsigned int*>(&data[65535] + 1 - 8);
		next_page = reinterpret_cast<unsigned int*>(&data[65535] + 1 - 4);
}
sdb::SlottedPage::SlottedPage(std::array<unsigned char, kPageSize>& _data) {
		//TODO: THE LRU CACHE SHARES OWNERSHIP IS IT SAFE TO MOVE?
		data = std::move(_data);
		footer = new Footer(static_cast<void*>(&data[65535] + 1 - 8));
		prev_page = reinterpret_cast<unsigned int*>(&data[65535] + 1 - 8);
		next_page = reinterpret_cast<unsigned int*>(&data[65535] + 1 - 4);
}
sdb::SlottedPage::~SlottedPage() {
		delete footer;
}
void* sdb::SlottedPage::getPageStart() {
	return static_cast<void*>(&data);
}
void* sdb::SlottedPage::getAddressOfElement(unsigned short index) {
		return static_cast<void*>(&data[index]);
}
//slot id uses 2 footer slots. one for pointer to the block. one for the size of the block
std::pair<void*, void*> sdb::SlottedPage::getBlock(int page_id) {
		unsigned short start = (*footer)[page_id];
		unsigned short end = start + (*footer)[page_id + 1];
		return { static_cast<void*>(&data[start]), static_cast<void*>(&data[end]) };
}
size_t sdb::SlottedPage::physicalSize() {
		unsigned short space = footer->getFreeSpacePtr();
		unsigned short footer_size = footer->physicalSize();
		return space + footer_size;
}

size_t sdb::SlottedPage::freeSpace() {
		return kPageSize - physicalSize();
}

size_t sdb::SlottedPage::size() {
		return footer->size() / 2;
}

std::tuple<void*, void*, unsigned short> sdb::SlottedPage::allocateBlock(size_t bytes_required) {
		unsigned short start_of_block = footer->getFreeSpacePtr();
		unsigned short end_of_block = footer->getFreeSpacePtr() + bytes_required;
		void* start_of_block_ptr = &data[start_of_block];
		void* end_of_block_ptr = &data[end_of_block];
		int insert_index = getEmptyFooterSlot();
		if (insert_index != -1) {
				(*footer)[insert_index] = start_of_block;
				(*footer)[insert_index + 1] = end_of_block - start_of_block;
				
				footer->setFreeSpace(end_of_block);
				return { start_of_block_ptr, end_of_block_ptr, static_cast<unsigned short>(insert_index) };
		}
		else {
				footer->push_back(start_of_block);
				footer->push_back(end_of_block - start_of_block);
				footer->setFreeSpace(end_of_block);
				return{ start_of_block_ptr, end_of_block_ptr, static_cast<unsigned short>(footer->size() - 2) };
		}
}
int sdb::SlottedPage::getEmptyFooterSlot() {
		for (int i = 0; i < footer->size(); ++i) {
				if ((*footer)[i] == kPageSize) {
						return i;
				}
		}
		return -1;
}

unsigned int sdb::SlottedPage::getNextPage() const {
		return *next_page;
}
unsigned int sdb::SlottedPage::getPrevPage() const {
		return *prev_page;
}
void sdb::SlottedPage::setNextPage(unsigned int index) {
		*next_page = index;
}
void sdb::SlottedPage::setPrevPage(unsigned int index) {
		*prev_page = index;
}