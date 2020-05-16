#include "argparse.h"

#include <cerrno>
#include <stdexcept>

namespace core {

ArgParser::ArgParser() = default;

ArgParser::~ArgParser() = default;

std::vector<std::string> ArgParser::tokenize(int argc, char *argv[]) const {
    std::vector<std::string> args{};

    for (int i = 0; i < argc; ++i) {
        args.push_back(argv[i]);
    }

    return args;
}

std::vector<TaxReturn>
ArgParser::parse_returns(std::vector<std::string> tokens) const {
    std::vector<TaxReturn> returns{};

    std::vector<IncomeSlice> slices{};
    int64_t base = 0L;

    for (std::size_t i = 1; i < tokens.size(); ++i) {
        std::string token{tokens[i]};

        int64_t number = parse_number(token);

        IncomeSlice slice{CashAmount{base}, CashAmount{number}};
        slices.push_back(slice);

        base = base + number;
    }

    if (slices.size() > 0) {
        TaxReturn taxret{slices};
        returns.push_back(taxret);
    }

    return returns;
}

std::vector<TaxReturn> ArgParser::parse(int argc, char *argv[]) const {
    auto tokens = tokenize(argc, argv);
    return parse_returns(tokens);
}

int64_t ArgParser::parse_number(std::string arg) const {
    const char *buf = arg.c_str();
    char *cursor = const_cast<char *>(buf);

    errno = 0;
    double number_f = std::strtod(buf, &cursor);
    auto num_read = static_cast<std::size_t>(cursor - buf);

    if (errno > 0) {
        throw std::invalid_argument(
            "invalid argument passed to parse_number()");
    }

    int64_t number = static_cast<int64_t>(number_f * 100.0) * 100L;

    bool invalid_arg{false};

    if (num_read == arg.size() - 1) {
        char suffix = arg[arg.size() - 1];
        if (suffix == 'k') {
            number = number * 1000;
        } else if (suffix == 'm') {
            number = number * 1000000;
        } else {
            invalid_arg = true;
        }
    } else if (num_read < arg.size()) {
        invalid_arg = true;
    }

    if (invalid_arg) {
        throw std::invalid_argument(
            "invalid argument passed to parse_number()");
    }

    return number;
}

} // namespace core