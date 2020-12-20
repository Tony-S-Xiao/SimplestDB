#include"simplestdb_test.h"
#include"simplestdb_dbrow.h"
#include"simplestdb_token.h"
#include"simplestdb_tableheaderrow.h"
#include"simplestdb_tablepointerrow.h"
#include"simplestdb_datarow.h"
#include"simplestdb_page.h"
#include"simplestdb_disk_manager.h"
#include"lru_cache.h"

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<cassert>

void sdb::test() {
  sdb::DiskManager disk_man;
  disk_man.open({ "test_db.sdb" });
  disk_man.zeroOutSlot(0);
  // DBRow test
  std::vector<std::byte> dbrow_test(sdb::DBRow::calcSizeRequired(std::string("unreal")), static_cast<std::byte>(0));
  sdb::DBRow db_test_row(&*dbrow_test.begin(), &*(dbrow_test.end() - 1) + 1);
  db_test_row.setTableName("unreal");
  assert(db_test_row.getTableName() == std::string("unreal"));
  db_test_row.setTableName("unre");  // setTableName simply overwrites exisiting char
  assert(db_test_row.getTableName() == std::string("unreal"));
  db_test_row.setPageId(1234);
  assert(db_test_row.getPageId() == 1234);
  db_test_row.setPageId(4321);
  assert(db_test_row.getPageId() == 4321);
  db_test_row.setPageId(0);
  assert(db_test_row.getPageId() == 0);
  db_test_row.setPageId(-1);
  assert(db_test_row.getPageId() == 4294967295);
  db_test_row.setTableName("unreal33");  // truncates strings that are too long
  assert(db_test_row.getPageId() == 4294967295);
  assert(db_test_row.getTableName() == std::string("unreal"));
  sdb::DBRow db_test_row_exisiting(&*dbrow_test.begin(), &*(dbrow_test.end() - 1) + 1);
  assert(db_test_row_exisiting.getTableName() == std::string("unreal"));
  assert(db_test_row_exisiting.getPageId() == 4294967295);
  // TableHeaderRow test
  std::vector<sdb::SQLType> types{ sdb::SQLType::VARCHAR, sdb::SQLType::VARCHAR, sdb::SQLType::INTEGER };
  std::vector<std::string> names{ {"names"}, {"emails"}, {"student id"} };
  std::vector<std::byte> tableheaderrow_data(sdb::TableHeaderRow::calcSizeRequired(names, types), static_cast<std::byte>(0));
  assert(tableheaderrow_data.size() == 24 + 2 * 3 + 2);
  sdb::TableHeaderRow tableheaderrow_test(&*tableheaderrow_data.begin(), &*(tableheaderrow_data.end() - 1) + 1);
  for (int i = 0; i < tableheaderrow_test.getNumOfCol(); ++i) {
    assert(tableheaderrow_test.getColumnName(i) == names[i]);
    assert(tableheaderrow_test.getColumnType(i) == types[i]);
  }
  sdb::TableHeaderRow tableheaderrow_test_exisiting(&*tableheaderrow_data.begin(), &*(tableheaderrow_data.end() - 1) + 1);
  for (int i = 0; i < tableheaderrow_test_exisiting.getNumOfCol(); ++i) {
    assert(tableheaderrow_test_exisiting.getColumnName(i) == names[i]);
    assert(tableheaderrow_test_exisiting.getColumnType(i) == types[i]);
  }
  // TablePointer test
  std::vector<std::byte> tablepointer_data(6, static_cast<std::byte>(0));
  sdb::TablePointerRow tablepointer_test(&*tablepointer_data.begin(), &*(tablepointer_data.end() - 1) + 1);
  assert(tablepointer_test.getPageId() == 0);
  assert(tablepointer_test.getSpaceAvailable() == 0);
  tablepointer_test.setPageId(999);
  tablepointer_test.setSpaceAvailable(999);
  assert(tablepointer_test.getPageId() == 999);
  assert(tablepointer_test.getSpaceAvailable() == 999);
  tablepointer_test.setSpaceAvailable(-1);
  assert(tablepointer_test.getSpaceAvailable() == 0xFFFF);
  // DataRow test
  std::vector<sdb::bool8_t> all_bool{ true, false, true, true };
  std::vector<int> all_integer{ 1,3,5,7,11,13 };
  std::vector<std::string> all_datetime{};
  std::vector<std::string> all_string{ std::string("tony"), std::string("tonyxiao"), std::string("afwefafds"),
    std::string("rf af 334r"), std::string("asdl.lkv.kv0  ") };
  std::vector<std::byte> datarow_data(sdb::DataRow::calcSizeRequired(all_bool, all_integer, all_datetime, all_string), static_cast<std::byte>(0));
  sdb::DataRow datarow_test(&*datarow_data.begin(), &*(datarow_data.end() - 1) + 1);
  datarow_test.loadData(all_bool, all_integer, all_datetime, all_string);

  for (int i = 0; i < all_bool.size(); ++i) {
    assert(all_bool[i] == datarow_test.getBoolean(i));
  }
  for (int i = 0; i < all_integer.size(); ++i) {
    assert(all_integer[i] == datarow_test.getInteger(i));
  }
  for (int i = 0; i < all_string.size(); ++i) {
    assert(all_string[i] == datarow_test.getVarChar(i));
  }
  sdb::DataRow datarow_test_existing(&*datarow_data.begin(), &*(datarow_data.end() - 1) + 1);
  for (int i = 0; i < all_bool.size(); ++i) {
    assert(all_bool[i] == datarow_test.getBoolean(i));
  }
  for (int i = 0; i < all_integer.size(); ++i) {
    assert(all_integer[i] == datarow_test.getInteger(i));
  }
  for (int i = 0; i < all_string.size(); ++i) {
    assert(all_string[i] == datarow_test.getVarChar(i));
  }
  // Page Test
  std::array<std::byte, sdb::kPageSize>* data_for_page = new std::array<std::byte, sdb::kPageSize>{};
  sdb::SlottedPage* test_page = new sdb::SlottedPage{ data_for_page };
  auto test_block_1 = test_page->allocateBlock(11111);
  auto test_block_2 = test_page->allocateBlock(1);
  auto test_block_3 = test_page->allocateBlock(0);
  assert(std::get<0>(test_block_1) == test_page->getPageStart());
  assert(std::get<1>(test_block_1) == test_page->getPageStart() + 11111);
  assert(std::get<2>(test_block_1) == 0);
  assert(std::get<0>(test_block_2) == test_page->getPageStart() + 11111);
  assert(std::get<1>(test_block_2) == test_page->getPageStart() + 11112);
  assert(std::get<2>(test_block_2) == 1);
  assert(std::get<0>(test_block_3) == nullptr);
  assert(std::get<1>(test_block_3) == nullptr);
  assert(std::get<2>(test_block_3) == 0);
  { // lazy existing test
    auto test_block_1 = test_page->getBlock(0);
    auto test_block_2 = test_page->getBlock(1);
    assert(std::get<0>(test_block_1) == test_page->getPageStart());
    assert(std::get<1>(test_block_1) == test_page->getPageStart() + 11111);
    assert(std::get<0>(test_block_2) == test_page->getPageStart() + 11111);
    assert(std::get<1>(test_block_2) == test_page->getPageStart() + 11112);
  }

  sdb::SlottedPage* test_page_existing = new sdb::SlottedPage{ &test_page->extract() };
  { // lazy existing test
    auto test_block_1 = test_page_existing->getBlock(0);
    auto test_block_2 = test_page_existing->getBlock(1);
    assert(std::get<0>(test_block_1) == test_page_existing->getPageStart());
    assert(std::get<1>(test_block_1) == test_page_existing->getPageStart() + 11111);
    assert(std::get<0>(test_block_2) == test_page_existing->getPageStart() + 11111);
    assert(std::get<1>(test_block_2) == test_page_existing->getPageStart() + 11112);
  }

  // lru cache test
  LRUCache<int, int>* cache = new LRUCache<int, int>(1000);
  for (int i = 0; i < 1000; ++i) {
    cache->insert(i, 2 * i);
  }
  for (int i = 0; i < 500; ++i) {
    assert(cache->find(i)->first == i);
    assert(cache->find(i)->second == 2 * i);
  }
  for (int i = 1000; i < 1500; ++i) {
    cache->insert(i, 2 * i);
  }
  for (int i = 1000; i < 1500; ++i) {
    assert(cache->find(i)->first == i);
    assert(cache->find(i)->second == 2 * i);
  }
  for (int i = 0; i < 500; ++i) {
    assert(cache->find(i)->first == i);
    assert(cache->find(i)->second == 2 * i);
  }
  for (int i = 500; i < 1000; ++i) {
    assert(cache->find(i) == cache->end());
  }
  // leak tests
  LRUCache<size_t, std::unique_ptr<sdb::SlottedPage>>* cache_unique = new LRUCache<size_t, std::unique_ptr<sdb::SlottedPage>>(7);
  for (int i = 0; i < 100000; ++i) {
    std::unique_ptr<sdb::SlottedPage> testing_page1(new sdb::SlottedPage(new std::array<std::byte, kPageSize>()));
    cache_unique->insert(i, std::move(testing_page1));
  }
  delete cache_unique;
  delete cache;
  disk_man.closeCurrFile();
  // diskmanager test
  disk_man.open("test_db.sdb");
  disk_man.zeroOutSlot(0);
  std::vector<std::string> data_strings{ "abc", "abcde", "aaaaaaaaaaaaa", "00" };
  std::vector<std::string> data_date_time{};
  std::vector<int> data_int{0, -1, 1, 1234, INT_MAX, INT_MIN};
  std::vector<bool8_t> data_bool{true, false};
  sdb::SlottedPage* page_zero = disk_man.readFromSlot(0);
  auto allocated_1 = page_zero->allocateBlock(sdb::DataRow::calcSizeRequired(data_bool, data_int, data_date_time, data_strings));
  sdb::DataRow row{ std::get<0>(allocated_1), std::get<1>(allocated_1) };
  row.loadData(data_bool, data_int, data_date_time, data_strings);
  assert(std::get<2>(allocated_1) == 0);
  assert(row.getVarChar(0) == data_strings[0]);
  assert(row.getVarChar(1) == data_strings[1]);
  assert(row.getVarChar(2) == data_strings[2]);
  assert(row.getVarChar(3) == data_strings[3]);
  assert(row.getInteger(0) == data_int[0]);
  assert(row.getInteger(1) == data_int[1]);
  assert(row.getInteger(2) == data_int[2]);
  assert(row.getInteger(3) == data_int[3]);
  assert(row.getInteger(4) == data_int[4]);
  assert(row.getInteger(5) == data_int[5]);
  assert(row.getBoolean(0) == data_bool[0]);
  assert(row.getBoolean(1) == data_bool[1]);
  disk_man.writeToSlot(page_zero, 0);
  disk_man.writeToSlot(page_zero, 1);
  disk_man.closeCurrFile();
  disk_man.open("test_db.sdb");
  sdb::SlottedPage* page_one = disk_man.readFromSlot(0);
  sdb::SlottedPage* page_two = disk_man.readFromSlot(1);
  auto block1 = page_one->getBlock(0);
  auto block2 = page_two->getBlock(0);
  sdb::DataRow row1{ std::get<0>(block1), std::get<1>(block1) };
  sdb::DataRow row2{ std::get<0>(block2), std::get<1>(block2) };
  assert(row1.getVarChar(0) == data_strings[0]);
  assert(row1.getVarChar(1) == data_strings[1]);
  assert(row1.getVarChar(2) == data_strings[2]);
  assert(row1.getVarChar(3) == data_strings[3]);
  assert(row1.getInteger(0) == data_int[0]);
  assert(row1.getInteger(1) == data_int[1]);
  assert(row1.getInteger(2) == data_int[2]);
  assert(row1.getInteger(3) == data_int[3]);
  assert(row1.getInteger(4) == data_int[4]);
  assert(row1.getInteger(5) == data_int[5]);
  assert(row1.getBoolean(0) == data_bool[0]);
  assert(row1.getBoolean(1) == data_bool[1]);
  assert(row2.getVarChar(0) == data_strings[0]);
  assert(row2.getVarChar(1) == data_strings[1]);
  assert(row2.getVarChar(2) == data_strings[2]);
  assert(row2.getVarChar(3) == data_strings[3]);
  assert(row2.getInteger(0) == data_int[0]);
  assert(row2.getInteger(1) == data_int[1]);
  assert(row2.getInteger(2) == data_int[2]);
  assert(row2.getInteger(3) == data_int[3]);
  assert(row2.getInteger(4) == data_int[4]);
  assert(row2.getInteger(5) == data_int[5]);
  assert(row2.getBoolean(0) == data_bool[0]);
  assert(row2.getBoolean(1) == data_bool[1]);
}