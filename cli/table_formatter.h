#ifndef CLI_TABLE_FORMATTER_H
#define CLI_TABLE_FORMATTER_H

#include <cstdint>
#include <string>
#include <vector>

#include "libcore/rule_items.h"
#include "librules/rules_registry.h"

namespace cli {

enum class ColumnType {
    CASH_AMOUNT,
    PERCENT,
    TEXT,
};

struct TableColumn {
    std::string heading;
    ColumnType type;
    uint8_t width;
    uint8_t leading_space;
};

class TableFormatter {
  public:
    TableFormatter(const rules::RulesRegistry *rules_registry,
                   const std::vector<TableColumn> &columns, char char_subtotal,
                   char char_total, bool skip_zero_payables);

    std::string format(const std::vector<core::RuleItems> &lines,
                       const core::LineItem &total,
                       const std::string &total_slug) const;

    std::string format_headers() const;
    std::string format_rule_line(const core::RuleItems &rule_items) const;
    std::string format_line(const core::LineItem &item,
                            const std::string &slug) const;
    std::string format_sep_subtotal() const;
    std::string format_sep_total() const;

  private:
    std::string format_separator(char ch) const;
    std::string format_char_sequence(char ch, uint8_t width) const;
    std::string align(bool align_left, uint8_t width, std::string text) const;
    bool should_align_left(const TableColumn &column) const;

    const rules::RulesRegistry *m_rules_registry{nullptr};
    const std::vector<TableColumn> &m_columns;
    char m_char_subtotal;
    char m_char_total;
    bool m_skip_zero_payables;
};

} // namespace cli

#endif // CLI_TABLE_FORMATTER_H
