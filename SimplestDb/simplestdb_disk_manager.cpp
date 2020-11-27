#include"simplestdb_disk_manager.h"
#include<iostream>

sdb::DiskManager::DiskManager() {
		cache = new LRUCache<int, sdb::SlottedPage*>(kDiskManagerCacheSize);
}

sdb::DiskManager::~DiskManager() {
		delete cache;
		file.close();
}

bool sdb::DiskManager::open(std::string filename)
{
	//must open in binary mode. windows adds extra characters if not in binary mode!
	file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
	if (!file.good()) {
		file.clear();
		file.open(filename, std::ios::out);
		file.close();
		file.open(filename, std::ios::out | std::ios::in | std::ios::binary);
	}
	return file.good();
}

void sdb::DiskManager::closeCurrFile() {
	file.close();
}

sdb::SlottedPage* sdb::DiskManager::readFromSlot(size_t index) {
		//lru cache deletes items in it
		if (cache->contains(index)) {
				std::cout << "LRU Cache Found Slot: " << index << std::endl;
				return cache->find(index)->second;
		}
		file.seekg(0, std::ios::end);
	if (file.good() && index * static_cast<long long int>(kPageSize) < file.tellg()) {
		std::array<unsigned char, kPageSize> *temp = new std::array<unsigned char, kPageSize>{};
		file.clear();
		file.seekg(index * static_cast<long long int>(kPageSize));
		file.read(reinterpret_cast<char*>(&(*temp)[0]), kPageSize);
		SlottedPage* result{ new SlottedPage(*temp) };
		cache->insert(index, result);
		return result;
	}
	return nullptr;
}

bool sdb::DiskManager::writeToSlot(SlottedPage* to_write, int index)
{
		file.seekp(0, std::ios::end);
	if (file.good()) {
		file.clear();
		file.seekp(index * static_cast<long long int>(kPageSize));
		file.write(reinterpret_cast<char*>(to_write->getPageStart()), kPageSize);
		//std::cout << "Wrote: " << index * static_cast<long long int>(kPageSize) << " " << file.good() << std::endl;
		return true;
	}
	return false;
}

bool sdb::DiskManager::append(SlottedPage* to_write) {
		if (file.good()) {
				file.clear();
				file.seekg(0, std::ios::end);
				file.write(reinterpret_cast<char*>(to_write->getPageStart()), kPageSize);
				//std::cout << "Append." << std::endl;
				return true;
		}
		return false;
}

void sdb::DiskManager::zeroOutSlot(size_t index) {
	if (file.good()) {
		file.clear();
		file.seekg(index * static_cast<long long int>(kPageSize));
		//std::cout << "ZERO: " << index * static_cast<long long int>(kPageSize) << std::endl;
		file.write(&kZeroPage[0], kPageSize);
	}
}