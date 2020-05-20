#include "table_formatter.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "libcore/defs.h"
#include "libcore/format.h"

namespace cli {

TableFormatter::TableFormatter(const rules::RulesRegistry *rules_registry,
                               const std::vector<TableColumn> &columns,
                               char char_subtotal, char char_total,
                               bool skip_zero_payables)
    : m_rules_registry{rules_registry}, m_columns{columns},
      m_char_subtotal{char_subtotal}, m_char_total{char_total},
      m_skip_zero_payables{skip_zero_payables} {
    if (m_columns.size() != 5) {
        throw std::invalid_argument("Must have exactly 5 columns");
    }
}

std::string TableFormatter::format(const std::vector<core::RuleItems> &lines,
                                   const core::LineItem &total,
                                   const std::string &total_slug) const {
    std::stringstream ss{};

    std::string headers = format_headers();
    std::string sep_subtotal = format_sep_subtotal();
    std::string sep_total = format_sep_total();

    ss << headers;
    ss << sep_subtotal;

    for (const core::RuleItems &rule_items : lines) {
        if (m_skip_zero_payables &&
            rule_items.net().payable() == core::CashAmount{0L}) {
            continue;
        }

        ss << format_rule_line(rule_items);
    }

    ss << sep_subtotal;
    ss << format_line(total, total_slug);
    ss << sep_total;

    return ss.str();
}

std::string TableFormatter::format_headers() const {
    std::stringstream ss{};

    for (const TableColumn &column : m_columns) {
        auto spaces = format_char_sequence(' ', column.leading_space);
        bool align_left = should_align_left(column);
        auto text = align(align_left, column.width, column.heading);
        ss << spaces << text;
    }

    ss << '\n';
    return ss.str();
}

std::string
TableFormatter::format_rule_line(const core::RuleItems &rule_items) const {
    const core::LineItem netted = rule_items.net();

    const core::Rule &rule = m_rules_registry->get_rule(rule_items.rule_id());
    auto rule_slug = rule.slug();

    return format_line(netted, rule_slug);
}

std::string TableFormatter::format_line(const core::LineItem &item,
                                        const std::string &slug) const {
    std::stringstream ss{};

    const TableColumn &tax_col = m_columns[0];
    const TableColumn &pay_col = m_columns[1];
    const TableColumn &perc_col = m_columns[2];
    const TableColumn &atax_col = m_columns[3];
    const TableColumn &rule_col = m_columns[4];

    auto tax_spaces = format_char_sequence(' ', tax_col.leading_space);
    bool tax_align_left = should_align_left(tax_col);
    auto tax_cd = core::CreditDebit::CREDIT;
    auto tax_amount = core::format_with_sign(tax_cd, item.taxable());
    auto tax_text = align(tax_align_left, tax_col.width, tax_amount);
    ss << tax_spaces << tax_text;

    auto pay_spaces = format_char_sequence(' ', pay_col.leading_space);
    bool pay_align_left = should_align_left(pay_col);
    auto pay_cd = item.credit_debit();
    auto pay_amount = core::format_with_sign(pay_cd, item.payable());
    auto pay_text = align(pay_align_left, pay_col.width, pay_amount);
    ss << pay_spaces << pay_text;

    auto perc_spaces = format_char_sequence(' ', perc_col.leading_space);
    bool perc_align_left = should_align_left(perc_col);
    auto perc_amount = core::format_percent(item.percent());
    auto perc_text = align(perc_align_left, perc_col.width, perc_amount);
    ss << perc_spaces << perc_text;

    auto atax_spaces = format_char_sequence(' ', atax_col.leading_space);
    bool atax_align_left = should_align_left(atax_col);
    auto atax_cd = core::CreditDebit::CREDIT;
    auto atax_amount = core::format_with_sign(atax_cd, item.after_tax());
    auto atax_text = align(atax_align_left, atax_col.width, atax_amount);
    ss << atax_spaces << atax_text;

    auto rule_spaces = format_char_sequence(' ', rule_col.leading_space);
    bool rule_align_left = should_align_left(rule_col);
    auto rule_text = align(rule_align_left, rule_col.width, slug);
    ss << rule_spaces << rule_text;

    ss << '\n';
    return ss.str();
}

std::string TableFormatter::format_sep_subtotal() const {
    if (m_char_subtotal > 0) {
        return format_separator(m_char_subtotal);
    }

    return "";
}

std::string TableFormatter::format_sep_total() const {
    if (m_char_total > 0) {
        return format_separator(m_char_total);
    }

    return "";
}

std::string TableFormatter::format_separator(char ch) const {
    std::stringstream ss{};

    for (const TableColumn &column : m_columns) {
        auto spaces = format_char_sequence(' ', column.leading_space);
        auto seq = format_char_sequence(ch, column.width);
        ss << spaces << seq;
    }

    ss << '\n';
    return ss.str();
}

std::string TableFormatter::format_char_sequence(char ch, uint8_t width) const {
    std::stringstream ss{};

    for (int i = 0; i < width; ++i) {
        ss << ch;
    }

    return ss.str();
}

std::string TableFormatter::align(bool align_left, uint8_t width,
                                  std::string text) const {
    std::stringstream ss{};
    if (align_left) {
        ss << std::left;
    } else {
        ss << std::right;
    }
    ss << std::setw(width);
    ss << text;
    return ss.str();
}

bool TableFormatter::should_align_left(const TableColumn &column) const {
    return column.type == ColumnType::TEXT ? true : false;
}

} // namespace cli