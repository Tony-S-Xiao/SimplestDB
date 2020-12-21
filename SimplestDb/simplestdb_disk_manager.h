#ifndef SIMPLESTDB_DISK_MANAGER_H_
#define SIMPLESTDB_DISK_MANAGER_H_
#include<fstream>
#include<string>
#include<array>
#include<cstddef>

#include"lru_cache.h"
#include"simplestdb_page.h"

namespace sdb {
// The capacity of the LRU cache in diskmanager.
constexpr int kDiskManagerCacheSize{ 1000 };
// Constant page full of zeros used to delete a slot in the database.
constexpr std::array<char, kPageSize> kZeroPage{};
// This object is used to load pages from the .sdb file into memory.
// This object should be the sole owner of pages.
class DiskManager {
public:
	DiskManager();
	~DiskManager();
	// Opens the file in binary mode input and output.
	bool open(std::string filename);
	// Closes the file.
	void closeCurrFile();
	// Opens a page in the slot in the .sdb file.
	SlottedPage* readFromSlot(size_t index);
	// Writes to a given slot.
	// Overwrites the existing page.
	// Deletes the page from memory after writing.
bool writeToSlot(SlottedPage* to_write, size_t index);
	// Writes to the end of the .sdb file.
	// Deletes the page from memory after writing.
	size_t append(SlottedPage* to_write);
	// Deletes the page at the given slot.
	// Writes a page of zeros.
	void zeroOutSlot(size_t index);
private:
	// Fstream object of the current open file.
	std::fstream file{};
	// Lru cache of opened pages.
	// Capacity hard coded in the kDiskManagerCacheSize.
	LRUCache<size_t, std::unique_ptr<sdb::SlottedPage>> *cache;
};
}  // namespace sdb
#endif // !SIMPLESTDB_DISK_MANAGER_H_