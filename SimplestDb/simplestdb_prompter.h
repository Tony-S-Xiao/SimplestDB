#pragma once
#include<string>
#include<unordered_map>
namespace sdb {

enum class PrompterCommand { NUL, HELP, INVALID, UNIMPLEMENTED };

class Prompter {
  public:
    Prompter();
    ~Prompter();
    void load(std::string);
    void print(PrompterCommand);
    void print(std::string);
  private:
    PrompterCommand convertToCommand(std::string);
    std::unordered_map<int, std::string> data;
};

} //namespace sdb