#include"simplestdb_row.h"
#include"simplestdb_page_footer.h"
#include"simplestdb_page.h"

#include<vector>
#include<algorithm>
#include<iostream>
#include<functional>
#include<utility>
#include<cstddef>
#include<iostream>

sdb::SlottedPage::SlottedPage(std::unique_ptr<std::array<std::byte, kPageSize>>&& data_ptr) :
		data_ptr_(std::move(data_ptr)) {
		next_page = reinterpret_cast<OnDiskPointer*>(&*((*data_ptr_).end() - sizeof(OnDiskPointer)));
		prev_page = next_page + 1;
		footer_ = new Footer(reinterpret_cast<std::byte*>(prev_page));
}
sdb::SlottedPage::SlottedPage(std::array<std::byte, kPageSize>* data_ptr) :
		data_ptr_(data_ptr) {
		next_page = reinterpret_cast<OnDiskPointer*>(&*((*data_ptr_).end() - sizeof(OnDiskPointer)));
		prev_page = next_page + 1;
		footer_ = new Footer(reinterpret_cast<std::byte*>(prev_page));
}
sdb::SlottedPage::SlottedPage(const SlottedPage& to_copy) :
data_ptr_(new std::array<std::byte, kPageSize>(*to_copy.data_ptr_))
{
		next_page = reinterpret_cast<OnDiskPointer*>(&*((*data_ptr_).end() - sizeof(OnDiskPointer)));
		prev_page = next_page + 1;
		footer_ = new Footer(reinterpret_cast<std::byte*>(prev_page));
}
sdb::SlottedPage::~SlottedPage() {
		delete footer_;
}
std::byte* sdb::SlottedPage::getPageStart() {
	 return data_ptr_->data();
}
std::array<std::byte, sdb::kPageSize>& sdb::SlottedPage::extract() {
		return *data_ptr_;
}
std::pair<std::byte*, std::byte*> sdb::SlottedPage::getBlock(OnPagePointer slot_id) {
		// Gets the pair of pointers representing the block.
		// Uses two numbers in the footer. one for location one for length
		OnPagePointer start = (*footer_)[2*slot_id];
		OnPagePointer end = start + (*footer_)[2*slot_id + 1];
		return { &(*data_ptr_)[start], &(*data_ptr_)[end] };
}
sdb::size16_t sdb::SlottedPage::physicalSize() {
		size16_t space = footer_->getFreeSpacePtr();
		size16_t footer_size = footer_->physicalSize();
		return space + footer_size;
}
sdb::size16_t sdb::SlottedPage::freeSpace() {
		return kPageSize - physicalSize();
}
sdb::size16_t sdb::SlottedPage::size() {
  // Uses two numbers in the footer_ for each block
		return footer_->size() / 2;
}
std::tuple<std::byte*, std::byte*, unsigned short> sdb::SlottedPage::allocateBlock(size16_t bytes_required) {
		if (bytes_required == 0) return { nullptr, nullptr, 0 };
		OnPagePointer start_of_block = footer_->getFreeSpacePtr();
		OnPagePointer end_of_block = footer_->getFreeSpacePtr() + bytes_required;
		std::byte* start_of_block_ptr = &(*data_ptr_)[start_of_block];
		std::byte* end_of_block_ptr = &(*data_ptr_)[end_of_block];
		footer_->push_back(start_of_block);
		footer_->push_back(end_of_block - start_of_block);
		footer_->setFreeSpace(end_of_block);
		return{ start_of_block_ptr, end_of_block_ptr, static_cast<unsigned short>(footer_->size() / 2 - 1) };
}
std::byte* sdb::SlottedPage::getAddressOfElement(OnPagePointer index) {
		return &(*data_ptr_)[index];
}
sdb::OnDiskPointer sdb::SlottedPage::getNextPage() const {
		return *next_page;
}
sdb::OnDiskPointer sdb::SlottedPage::getPrevPage() const {
		return *prev_page;
}
void sdb::SlottedPage::setNextPage(OnDiskPointer index) {
		*next_page = index;
}
void sdb::SlottedPage::setPrevPage(OnDiskPointer index) {
		*prev_page = index;
}
size_t sdb::SlottedPage::getPageId() {
		return page_id_;
}
void sdb::SlottedPage::setPageID(size_t page_id) {
		page_id_ = page_id;
}