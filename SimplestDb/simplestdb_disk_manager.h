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
	//opens a page in the slot in the .sdb file
	SlottedPage* readFromSlot(size_t index);
	//writes to a given slot
	//overwrites the existing page
	bool writeToSlot(SlottedPage* to_write, int index);
	//writes to the end of the .sdb file
	bool append(SlottedPage* to_write);
	//'deletes' the page at the given slot
	//writes a page of zeros
	void zeroOutSlot(size_t index);
private:
	//fstream object of the current open file
	std::fstream file{};
	//lru cache of opened pages
	//capacity hard coded in the kDiskManagerCacheSize
	LRUCache<size_t, sdb::SlottedPage*> *cache;
};
}  // namespace sdb
#endif // !SIMPLESTDB_DISK_MANAGER_H_