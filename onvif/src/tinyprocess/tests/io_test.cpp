#include "process.hpp"
#include <cassert>
#include <iostream>

int main() {
  auto output=std::make_shared<std::string>();
  auto error=std::make_shared<std::string>();
  {
    Process process("echo Test", "", [output](const char *bytes, size_t n) {
      *output+=std::string(bytes, n);
    });
    assert(process.get_exit_status()==0);
    assert(output->substr(0, 4)=="Test");
    output->clear();
  }
  
#ifndef _WIN32
  {
    Process process([] {
      std::cout << "Test" << std::endl;
      exit(0);
    }, [output](const char *bytes, size_t n) {
      *output+=std::string(bytes, n);
    });
    assert(process.get_exit_status()==0);
    assert(output->substr(0, 4)=="Test");
    output->clear();
  }
#endif
  
  {
    Process process("echo Test && ls an_incorrect_path", "", [output](const char *bytes, size_t n) {
      *output+=std::string(bytes, n);
    }, [error](const char *bytes, size_t n) {
      *error+=std::string(bytes, n);
    });
    assert(process.get_exit_status()>0);
    assert(output->substr(0, 4)=="Test");
    assert(!error->empty());
    output->clear();
    error->clear();
  }
  
  {
    Process process("bash", "", [output](const char *bytes, size_t n) {
      *output+=std::string(bytes, n);
    }, nullptr, true);
    process.write("echo Test\n");
    process.write("exit\n");
    assert(process.get_exit_status()==0);
    assert(output->substr(0, 4)=="Test");
    output->clear();
  }
  
  {
    Process process("cat", "", [output](const char *bytes, size_t n) {
      *output+=std::string(bytes, n);
    }, nullptr, true);
    process.write("Test\n");
    process.close_stdin();
    assert(process.get_exit_status()==0);
    assert(output->substr(0, 4)=="Test");
    output->clear();
  }
}
