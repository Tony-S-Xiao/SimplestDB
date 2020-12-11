#include"simplestdb_row.h"
#include"simplestdb_page_footer.h"
#include"simplestdb_page.h"

#include<vector>
#include<algorithm>
#include<iostream>
#include<functional>
#include<utility>
#include<cstddef>

sdb::SlottedPage::SlottedPage() {
		footer = new Footer(static_cast<std::byte*>(&data_[65535] + 1 - sizeof(*next_page) - sizeof(*prev_page)));
		prev_page = reinterpret_cast<OnDiskPointer*>(&data_[65535] + 1 - sizeof(*next_page) - sizeof(*prev_page));
		next_page = reinterpret_cast<OnDiskPointer*>(&data_[65535] + 1 - sizeof(*next_page));
}
sdb::SlottedPage::SlottedPage(std::array<std::byte, kPageSize>&& data) : 
data_{std::move(data)}	{ //TODO: THE LRU CACHE SHARES OWNERSHIP IS IT SAFE TO MOVE?
		footer = new Footer(static_cast<std::byte*>(&data_[65535] + 1 - sizeof(*next_page) - sizeof(*prev_page)));
		prev_page = reinterpret_cast<OnDiskPointer*>(&data_[65535] + 1 - sizeof(*next_page) - sizeof(*prev_page));
		next_page = reinterpret_cast<OnDiskPointer*>(&data_[65535] + 1 - sizeof(*next_page));
}
sdb::SlottedPage::~SlottedPage() {
		delete footer;
}
std::byte* sdb::SlottedPage::getPageStart() {
	return data_.data();
}
std::byte* sdb::SlottedPage::getAddressOfElement(OnPagePointer index) {
		return &data_[index];
}
//slot id uses 2 footer slots. one for pointer to the block. one for the size of the block
std::pair<std::byte*, std::byte*> sdb::SlottedPage::getBlock(OnPagePointer page_id) {
		OnPagePointer start = (*footer)[page_id];
		OnPagePointer end = start + (*footer)[page_id + 1];
		return { static_cast<std::byte*>(&data_[start]), static_cast<std::byte*>(&data_[end]) };
}
sdb::size16_t sdb::SlottedPage::physicalSize() {
		OnPagePointer space = footer->getFreeSpacePtr();
		OnPagePointer footer_size = footer->physicalSize();
		return space + footer_size;
}

sdb::size16_t sdb::SlottedPage::freeSpace() {
		return static_cast<size16_t>(kPageSize - physicalSize());
}

sdb::size16_t sdb::SlottedPage::size() {
		return footer->size() / 2;
}

std::tuple<std::byte*, std::byte*, sdb::OnPagePointer> sdb::SlottedPage::allocateBlock(size16_t bytes_required) {
		if (bytes_required == 0) return { nullptr, nullptr, 0 };
		OnPagePointer start_of_block = footer->getFreeSpacePtr();
		OnPagePointer end_of_block = footer->getFreeSpacePtr() + bytes_required;
		std::byte* start_of_block_ptr = &data_[start_of_block];
		std::byte* end_of_block_ptr = &data_[end_of_block];
		size16_t insert_index = static_cast<size16_t>(getEmptyFooterSlot());
		if (insert_index != -1) {
				(*footer)[insert_index] = start_of_block;
				(*footer)[insert_index + 1] = end_of_block - start_of_block;
				
				footer->setFreeSpace(end_of_block);
				return{ start_of_block_ptr, end_of_block_ptr, static_cast<unsigned short>(insert_index) };
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
				if ((*footer)[static_cast<size16_t>(i)] == kPageSize) {
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
void sdb::SlottedPage::setNextPage(OnDiskPointer index) {
		*next_page = index;
}
void sdb::SlottedPage::setPrevPage(OnDiskPointer index) {
		*prev_page = index;
}