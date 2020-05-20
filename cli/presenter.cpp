#include "presenter.h"

#include <sstream>

#include "libcore/format.h"
#include "libcore/income_slice.h"
#include "table_formatter.h"

namespace cli {

Presenter::Presenter(const rules::RulesRegistry *rules_registry,
                     const rules::TaxCalculation &calc)
    : m_rules_registry{rules_registry}, m_calc{calc} {}

std::string Presenter::present() const {
    auto total_inc = m_calc.return_total();
    auto fmt_total_inc = format_with_commas(total_inc.taxable());
    auto amt_width = 1 + fmt_total_inc.size();

    std::vector<TableColumn> columns{
        {"Taxable", ColumnType::CASH_AMOUNT, amt_width, 2},
        {"Payable", ColumnType::CASH_AMOUNT, amt_width, 2},
        {"%", ColumnType::PERCENT, 5, 2},
        {"After tax", ColumnType::CASH_AMOUNT, amt_width, 2},
        {"Rule", ColumnType::TEXT, 20, 2},
    };
    TableFormatter formatter{m_rules_registry, columns, '-', '\0', true};

    std::stringstream ss{};
    int sliceno = 1;

    for (const core::IncomeSlice &slice : m_calc.slices()) {
        auto items = m_calc.get_ruleitems(slice);
        auto total = m_calc.slice_total(slice);
        std::string slice_block = formatter.format(items, total, "Total");

        ss << format_slice_header(sliceno++, slice);
        ss << '\n';
        ss << slice_block;
        ss << '\n';
    }

    auto items = m_calc.net_slices();
    std::string return_block = formatter.format(items, total_inc, "Total");

    ss << "Total :: " << fmt_total_inc << '\n';
    ss << '\n';
    ss << return_block;
    ss << '\n';

    return ss.str();
}

std::string
Presenter::format_slice_header(int sliceno,
                               const core::IncomeSlice &slice) const {
    std::stringstream ss{};

    ss << "Slice " << sliceno;
    ss << " :: ";
    ss << format_with_commas(slice.amount());
    ss << "  [ ";
    ss << format_with_commas(slice.base());
    ss << " - ";
    ss << format_with_commas(slice.upper_bound());
    ss << " ]\n";

    return ss.str();
}

} // namespace cli
