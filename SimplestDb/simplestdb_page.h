#ifndef SIMPLESTDB_PAGE_H_
#define SIMPLESTDB_PAGE_H_
#include"simplestdb_row.h"
#include"simplestdb_page_footer.h"

#include<cstddef>
#include<cstdint>
#include<array>
#include<utility>
#include<memory>

namespace sdb {
// This pointer indicates the position of a std::byte on the page.
// Pointer is the index of the byte 0-indexed.
using OnPagePointer = uint16_t;
// Used to indicate the size of the blocks that have been allocated.
// Only needs to be as big as 2^16.
using size16_t = uint16_t;
// This class acts as a std::vector<unsigned short> that resides on a page object.
// Grows from higher address to lower address(backwards).
// Helps with keeping track of data blocks on page objects.
// Hard coded page size 2^16 bytes.
constexpr size_t kPageSize{ 65536 };
// This is the main object responsible for the physical layout of the page.
// All data in the database should go on these pages.
// The page object allocates a block of bytes on the page.
// The footer object is used to keep track of the positions of the blocks.
class SlottedPage {
public:
		// Takes ownership of the array of bytes.
		SlottedPage(std::unique_ptr<std::array<std::byte, kPageSize>>&&);
  SlottedPage(std::array<std::byte, kPageSize>*);
		SlottedPage(const SlottedPage& to_copy);
		~SlottedPage();
		// Gets the address of the array within the page.
		// Used to write the block of data into the disk.
		std::byte* getPageStart();
		// Gets the array.
		std::array<std::byte, kPageSize>& extract();
		// Gets the block by slot id. Returns [start of block, end of block).
		std::pair<std::byte*, std::byte*> getBlock(OnPagePointer slot_id);
		// Gets the size in bytes of the total space used on the page.
		size16_t physicalSize();
		// Gets the size in bytes of free space on the page.
		size16_t freeSpace();
		// Gets the size in number of slots used.
		size16_t size();
		// Returns [start, end), slot id of allocated block. returns nullptr, nullptr, 0 for size 0.
		std::tuple<std::byte*, std::byte*, unsigned short> allocateBlock(size16_t bytes_required);
		// Used to implement doublely-linked list. Should default to 0 if no next/prev page.
		OnDiskPointer getNextPage() const;
		OnDiskPointer getPrevPage() const;
		void setNextPage(OnDiskPointer index);
		void setPrevPage(OnDiskPointer index);
		size_t getPageId();
		void setPageID(size_t page_id);
private:
		std::unique_ptr<std::array<std::byte, kPageSize>> data_ptr_{};
		// Footer used to keep track of the location of blocks.
		Footer* footer_{ nullptr };
		// These pointers implement a on-disk doublely-linked list.
		OnDiskPointer* next_page{ nullptr };
		OnDiskPointer* prev_page{ nullptr };
		// Gets the address of one byte on the page.
		std::byte* getAddressOfElement(OnPagePointer index);
		// This is the page id of the current page.
		// Initialized when read from disk by disk manager.
		size_t page_id_{ 0 };
};
} // namespace sdb
#endif // !SIMPLESTDB_PAGE_H_