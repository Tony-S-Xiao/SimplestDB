#include"simplestdb_disk_manager.h"
#include"lru_cache.h"
#include"simplestdb_page.h"

#include<iostream>
#include<cstddef>

sdb::DiskManager::DiskManager() {
		cache = new LRUCache<size_t, std::unique_ptr<sdb::SlottedPage>>(kDiskManagerCacheSize);
}
sdb::DiskManager::~DiskManager() {
		delete cache;
		file.close();
}

bool sdb::DiskManager::open(std::string filename)
{
	// Must open in binary mode. Windows adds extra characters if not in binary mode!
	file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
	if (!file.good()) {
			file.clear();
			file.open(filename, std::ios::out | std::ios::in | std::ios::binary | std::ios::trunc);
	}
	return file.good();
}

void sdb::DiskManager::closeCurrFile() {
	file.close();
}

sdb::SlottedPage* sdb::DiskManager::readFromSlot(size_t index) {
		if (cache->contains(index)) {
				return cache->find(index)->second.get();
		}
		file.seekg(0, std::ios::end);
	if (file.good() && index * static_cast<long long int>(kPageSize) < file.tellg()) {
		std::array<std::byte, kPageSize>* temp{ new std::array<std::byte, kPageSize>{} };
		file.clear();
		file.seekg(index * static_cast<long long int>(kPageSize), std::ios::beg);
		file.read(reinterpret_cast<char*>(&(temp)[0]), kPageSize);
		std::unique_ptr<SlottedPage> result{ new SlottedPage(temp) };
		result->setPageID(index);
		cache->insert(index, std::move(result));
		return cache->find(index)->second.get();
	}
	return nullptr;
}

bool sdb::DiskManager::writeToSlot(SlottedPage* to_write, size_t index)
{
		file.seekp(0, std::ios::beg);
	if (file.good()) {
		file.seekp(index * static_cast<long long int>(kPageSize), std::ios::beg);
		file.write(reinterpret_cast<char*>(to_write->getPageStart()), kPageSize);
		cache->erase(index);
		return true;
	}
	return false;
}

size_t sdb::DiskManager::append(SlottedPage* to_write) {
		if (file.good()) {
				file.clear();
				file.seekp(0, std::ios::end);
				writeToSlot(to_write, file.tellp() / kPageSize);
				file.seekp(0, std::ios::end);
				return file.tellp() / kPageSize - 1;
		}
		return 0;
}

void sdb::DiskManager::zeroOutSlot(size_t index) {
	if (file.good()) {
		file.clear();
		file.seekg(index * static_cast<long long int>(kPageSize));
		file.write(&kZeroPage[0], kPageSize);
	}
}
bool sdb::DiskManager::isOpen() {
		return file.is_open();
}