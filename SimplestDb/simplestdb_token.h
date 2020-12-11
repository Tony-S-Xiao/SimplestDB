#ifndef SIMPLESTDB_TOKEN_H_
#define SIMPLESTDB_TOKEN_H_

#include<string>
#include<vector>

namespace sdb {

enum class SQLType : unsigned char { 
  NUL = 0,
  VARCHAR = 1 << 0,
  BOOLEAN = 1 << 1,
  INTEGER = 1 << 2,
  DATETIME = 1 << 3
};
enum class TokenType : unsigned char { 
  NUL = 0,
  READ = 1 << 0,
  WRITE = 1 << 1,
  NEW = 1 << 2,
  OPEN = 1 << 3,
  CD = 1 << 4,
  HELP = 1 << 5,
  CREATE = 1 << 6
};

class Token {
 public:
   void setTokenType(sdb::TokenType);
   TokenType getTokenType();
   void setWellFormedFlag(bool);
   bool getWellFormedFlag();
   ~Token();
   //metatoken override
   virtual void appendData(std::string) { return; };
   virtual void appendCommand(std::string) { return; };
   virtual std::vector<std::string>* getData() { return nullptr; };
   virtual std::vector<std::string>* getCommand() { return nullptr; };
   //sqltoken override
   virtual void setName(std::string) { return; };
   virtual std::string getName() { return""; };
   virtual void pushBackColumnName(std::string) { return; };
   virtual void pushBackColumnType(SQLType) { return; };
   virtual std::vector<std::string>* getColumnNames() { return nullptr; };
   virtual std::vector<SQLType>* getColumnTypes() { return nullptr; };
 protected:
   Token();
 private:
   bool well_formed{ false };
   TokenType token_type{ TokenType::NUL };
};

class MetaToken : public Token {
  public:
    MetaToken();
    ~MetaToken();
    void appendData(std::string);
    std::vector<std::string>* getData();
  protected:
  private:
    std::vector<std::string> data;
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

#endif // !SIMPLESTDB_TOKEN_H_