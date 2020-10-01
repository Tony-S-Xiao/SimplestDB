#pragma once
#include"simplestdb_row.h"

#include<iostream>
//creates an empty row with an id
sdb::Row::Row(uint32_t id, Table* table_header, unsigned char* free_start, unsigned char* free_end)
	: data_start(free_start), data_end(data_start), free_end(free_end), header(table_header) {
	void* write_pointer = static_cast<void*>(&id);
	unsigned char* byte_pointer = static_cast<unsigned char*>(write_pointer);
	unsigned char* trav_pointer = data_start;
	//write the id to the first 4 bytes of 
	*trav_pointer = *byte_pointer;
	++trav_pointer;
	++byte_pointer;
	*trav_pointer = *byte_pointer;
	++trav_pointer;
	++byte_pointer;
	*trav_pointer = *byte_pointer;
	++trav_pointer;
	++byte_pointer;
	*trav_pointer = *byte_pointer;
	++trav_pointer;
	++byte_pointer;
	data_end = trav_pointer;
}
//initialize an existing row on a page
sdb::Row::Row(Table* table_header, unsigned char* data_start, unsigned char* data_end)
	: data_start(data_start), data_end(data_end), free_end(data_end), header(table_header) {

}
//trivial destructor
sdb::Row::~Row() {

}

template <typename T>
T sdb::Row::get(std::string) {

}

template <typename T>
bool sdb::Row::set(std::string, T) {

}

void sdb::Row::print() {

}
//removes free space from row
void sdb::Row::pack() {
	free_end = data_end;
}

void sdb::Row::giveSpace(unsigned char* new_free_space_start, unsigned char* new_free_space_end) {

}

bool sdb::Row::move(unsigned char* new_location_start, unsigned char* new_location_end) {
	if (new_location_end - new_location_start < data_end - data_start)
		return false;

	unsigned char* new_trav = new_location_start;
	unsigned char* old_trav = data_start;
	while (old_trav != data_end) {
		*old_trav = *new_trav;
		++old_trav;
		++new_trav;
	}
	return true;
}
//returns size as an number of bytes
size_t sdb::Row::size() {
	return data_end - data_start;
}