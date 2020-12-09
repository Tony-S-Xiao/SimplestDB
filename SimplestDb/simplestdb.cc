#include"simplestdb_page_footer.h"
#include"simplestdb_page.h"
#include"simplestdb_disk_manager.h"
#include"simplestdb_row.h"

#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<cassert>

void test();

int main()
{
  test();

  return 0;
}

void test() {
  // DBRow test
  std::vector<std::byte> dbrow_test(sdb::DBRow::calcSizeRequired(std::string("unreal")), static_cast<std::byte>(0));
  sdb::DBRow db_test_row(&*dbrow_test.begin(), &*(dbrow_test.end()-1)+1);
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
  std::vector<sdb::SQLType> types{sdb::SQLType::VARCHAR, sdb::SQLType::VARCHAR, sdb::SQLType::INTEGER};
  std::vector<std::string> names{ {"names"}, {"emails"}, {"student id"} };
  std::vector<std::byte> tableheaderrow_data(sdb::TableHeaderRow::calcSizeRequired(names, types), static_cast<std::byte>(0));
  assert(tableheaderrow_data.size() == 24 + 2*3+2);
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
  std::vector<sdb::bool8_t> all_bool{true, false, true, true};
  std::vector<int> all_integer{1,3,5,7,11,13};
  std::vector<std::string> all_datetime{};
  std::vector<std::string> all_string{std::string("tony"), std::string("tonyxiao"), std::string("afwefafds"),
    std::string("rf af 334r"), std::string("asdl.lkv.kv0  ")};
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
}