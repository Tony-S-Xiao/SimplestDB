#pragma once
#include<string>
#include<vector>

namespace sdb {

enum class SQLType { VARCHAR, INTEGER, DATETIME, BOOLEAN };

class Token {
 public:
   void setTokenType(std::string);
   std::string getTokenType();
   ~Token();
   ////metatoken override
   //virtual void appendData(std::string) = 0;
   //virtual void appendCommand(std::string) = 0;
   //virtual std::vector<std::string>* getData() = 0;
   //virtual std::vector<std::string>* getCommand() = 0;
   //sqltoken override
   virtual void setName(std::string) { return; };
   virtual std::string getName() { return""; };
   virtual void pushBackColumnName(std::string) { return; };
   virtual void pushBackColumnType(SQLType) { return; };
   virtual std::vector<std::string>* getColumnNames() { return nullptr; };
   virtual std::vector<SQLType>* getColumnTypes() { return nullptr; };
   ////writetoken override
   //virtual void appendData(std::string) = 0;
   //virtual std::vector<std::string>* getData() = 0;
   ////querytoken override
   //virtual void setCondition(std::string) = 0;
   //virtual std::string getCondition() = 0;
 protected:
   Token();
 private:
     std::string token_type;
};

class MetaToken : public Token {
  public:
    MetaToken();
    ~MetaToken();
    void appendData(std::string);
    void appendCommand(std::string);
    std::vector<std::string>* getData();
    std::vector<std::string>* getCommand();
  protected:
  private:
      std::vector<std::string> data;
      std::vector<std::string> command;
};

class SQLToken : public Token {
  public:
    void setName(std::string);
    std::string getName();
    void pushBackColumnName(std::string);
    void pushBackColumnType(SQLType);
    std::vector<std::string>* getColumnNames();
    std::vector<SQLType>* getColumnTypes();
    ~SQLToken();
  protected:
    SQLToken();
  private:
    std::string table_name;
    std::vector<std::string> column_names;
    std::vector<SQLType> column_types;
};

class WriteToken : public SQLToken {
  public:
    WriteToken();
    ~WriteToken();
    void appendData(std::string);
    std::vector<std::string>* getData();
  protected:

  private:
    std::vector<std::string> data;
};

class QueryToken : public SQLToken {
  public:
      QueryToken();
      ~QueryToken();
      void setCondition(std::string);
      std::string getCondition();
  protected:
  private:
      std::string condition;
};

class CreateTableToken : public SQLToken {
  public:
    CreateTableToken();
    ~CreateTableToken();
  protected:
  private:
};

}//namespace sdb

