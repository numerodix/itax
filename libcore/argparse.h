#ifndef LIBCORE_ARGPARSE_H
#define LIBCORE_ARGPARSE_H

#include <string>
#include <vector>

#include "tax_return.h"

namespace core {

class ArgParser {
  public:
    ArgParser();
    ~ArgParser();

    std::vector<std::string> tokenize(int argc, char *argv[]) const;
    std::vector<TaxReturn> parse_returns(std::vector<std::string> tokens) const;

    std::vector<TaxReturn> parse(int argc, char *argv[]) const;

  private:
    int64_t parse_number(std::string arg) const;
};

} // namespace core

#endif // LIBCORE_ARGPARSE_H
