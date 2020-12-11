#ifndef SIMPLESTDB_DISK_MANAGER_H_
#define SIMPLESTDB_DISK_MANAGER_H_

#include<fstream>
#include<string>
#include<array>

#include"lru_cache.h"
#include"simplestdb_page.h"

/*
this object is responsible for loading pages from .sdb file into memory

*/

namespace sdb {

//the capacity of the LRU cache in diskmanager
constexpr int kDiskManagerCacheSize{ 100 };
//constant page full of zeros used to delete a slot in the database
constexpr std::array<char, kPageSize> kZeroPage{};

class DiskManager {
public:
 //initialize an LRU cache of pages
 //TODO: allow specifying LRU cache size?
	DiskManager();
	//deletes the LRU cache
	~DiskManager();
	//opens the file in binary mode input and output
	//the file must be opened before any other methods can be run
	//the constructor does not open a file
	bool open(std::string);
	//closes the file
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
	LRUCache<int, sdb::SlottedPage*> *cache;
};
}  // namespace sdb
#endif // !SIMPLESTDB_DISK_MANAGER_H_