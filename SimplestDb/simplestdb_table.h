#pragma once
#include"simplestdb_row.h"

#include<vector>
#include<string>


namespace sdb {

class Table {
  public:
    Table(std::string&, std::vector<SQLTypes>&);
    ~Table();

  private:
    std::vector<SQLTypes> header;
    std::string name;
    std::map<int, Row*> index;
};

}//namespace sdb