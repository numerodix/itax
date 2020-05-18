#include <iomanip>
#include <iostream>
#include <sstream>

#include "libcore/argparse.h"
#include "libcore/format.h"
#include "librules/rules_registry.h"
#include "librules/rulesets_registry.h"

using namespace core;
using namespace rules;
using namespace std;

std::string fmt(CreditDebit cd) {
    return cd == CreditDebit::CREDIT ? " " : "-";
}

std::string join(std::string prefix, std::string number) {
    stringstream ss{};
    ss << prefix << number;
    return ss.str();
}

std::ostream &pctfmt(std::ostream &out) {
    out << right << setfill(' ') << setw(5);
    return out;
}

std::ostream &numfmt(std::ostream &out) {
    out << right << setfill(' ') << setw(12);
    return out;
}

int main(int argc, const char *argv[]) {
    ArgParser parser{};
    TaxReturn taxret = parser.parse(argc, argv)[0];

    auto rules_registry = RulesRegistry::instance();
    auto ruleset_registry = RulesetsRegistry::instance();

    const Ruleset &ruleset = ruleset_registry->get_ruleset("aus-2020");

    TaxCalculation calc = ruleset.apply(taxret);
    auto slices = calc.slices();

    cout << "Applying ruleset: " << ruleset.desc() << "\n";
    cout << "\n";

    // Display each slice

    int slice_no = 0;
    for (auto slice : slices) {
        cout << "Slice " << ++slice_no
             << " :: " << format_with_commas(slice.amount()) << "  [ "
             << format_with_commas(slice.lower_bound()) << " - "
             << format_with_commas(slice.upper_bound()) << " ]" << '\n';

        cout << numfmt << "Taxable";
        cout << "  ";
        cout << numfmt << "Payable";
        cout << "  ";
        cout << pctfmt << "%";
        cout << "  ";
        cout << numfmt << "After tax";
        cout << "  ";
        cout << "Rule";
        cout << "\n";

        // Itemize rules for the slice

        auto rule_items_vec = calc.get_ruleitems(slice);
        for (const auto &rule_items : rule_items_vec) {
            const Rule &rule = rules_registry->get_rule(rule_items.rule_id());

            std::string prefix = fmt(rule_items.net().credit_debit());
            auto payable =
                join(prefix, format_with_commas(rule_items.net().payable()));

            auto pct = format_percent(rule_items.net().percent());

            cout << numfmt << format_with_commas(rule_items.net().taxable());
            cout << "  ";
            cout << numfmt << payable;
            cout << "  ";
            cout << pctfmt << pct;
            cout << "  ";
            cout << numfmt << format_with_commas(rule_items.net().after_tax());
            cout << "  ";
            cout << rule.slug();
            cout << "\n";
        }

        // Total for the slice

        auto slice_total = calc.slice_total(slice);

        std::string prefix = fmt(slice_total.credit_debit());
        auto payable = join(prefix, format_with_commas(slice_total.payable()));

        auto pct = format_percent(slice_total.percent());

        cout << numfmt << format_with_commas(slice_total.taxable());
        cout << "  ";
        cout << numfmt << payable;
        cout << "  ";
        cout << pctfmt << pct;
        cout << "  ";
        cout << numfmt << format_with_commas(slice_total.after_tax());
        cout << "  ";
        cout << "Total";
        cout << "\n";

        cout << "\n";
    }

    // Display return as a whole

    LineItem return_total = calc.return_total();
    cout << "Total"
         << " :: " << format_with_commas(return_total.taxable()) << "\n";

    cout << numfmt << "Taxable";
    cout << "  ";
    cout << numfmt << "Payable";
    cout << "  ";
    cout << pctfmt << "%";
    cout << "  ";
    cout << numfmt << "After tax";
    cout << "  ";
    cout << "Rule";
    cout << "\n";

    // Display total for each rule

    auto slice_totals = calc.net_slices();
    for (auto rule_items : slice_totals) {
        const Rule &rule = rules_registry->get_rule(rule_items.rule_id());

        std::string prefix = fmt(rule_items.net().credit_debit());
        auto payable =
            join(prefix, format_with_commas(rule_items.net().payable()));

        auto pct = format_percent(rule_items.net().percent());

        cout << numfmt << format_with_commas(rule_items.net().taxable());
        cout << "  ";
        cout << numfmt << payable;
        cout << "  ";
        cout << pctfmt << pct;
        cout << "  ";
        cout << numfmt << format_with_commas(rule_items.net().after_tax());
        cout << "  ";
        cout << rule.slug();
        cout << "\n";
    }

    // Total for the return

    std::string prefix = fmt(return_total.credit_debit());
    auto payable = join(prefix, format_with_commas(return_total.payable()));

    auto pct = format_percent(return_total.percent());

    cout << numfmt << format_with_commas(return_total.taxable());
    cout << "  ";
    cout << numfmt << payable;
    cout << "  ";
    cout << pctfmt << pct;
    cout << "  ";
    cout << numfmt << format_with_commas(return_total.after_tax());
    cout << "  ";
    cout << "Total";
    cout << "\n";
}