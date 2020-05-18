#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "libcore/argparse.h"
#include "libcore/bracket.h"
#include "libcore/format.h"
#include "libcore/rule.h"
#include "libcore/shortcuts.h"
#include "librules/ruleset.h"

using namespace core;
using namespace rules;
using namespace std;

RuleSet build_ruleset() {

    // Rule 1

    std::string slug1{"Medicare levy"};
    std::string desc1{"Medicare levy is 2% on all income"};
    FnCalc fn1 = FN_CALC_SIGNATURE {
        CashAmount taxable = slice.amount();
        CashAmount payable = taxable * 0.02;
        LineItem item{taxable, payable, CreditDebit::DEBIT};
        return {item};
    };

    Rule medicare_levy{801, slug1, desc1, fn1};

    // Rule 2

    std::string slug2{"Medicare levy surcharge"};
    std::string desc2{"Medicare levy surcharge is 1.5% on all income if total "
                      "income above 90k"};
    FnCalc fn2 = FN_CALC_SIGNATURE {
        if (taxret.total_income() > C(90000)) {
            CashAmount taxable = slice.amount();
            CashAmount payable = taxable * 0.015;
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }
        return {};
    };

    Rule medicare_surcharge{802, slug2, desc2, fn2};

    // Bracket Rule 3

    std::string slug3{"Bracket 0 - 18.2k"};
    std::string desc3{"0% on income 0k - 18.2k"};
    Bracket brac3{C(0), C(18200)};
    FnCalc fn3 = FN_CALC_SIGNATURE {
        CashAmount taxable = brac3.in_bracket(slice);
        CashAmount payable = taxable * 0.0;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule std_bracket1{101, slug3, desc3, fn3};

    // Bracket Rule 4

    std::string slug4{"Bracket 18.2k - 37k"};
    std::string desc4{"19% on income 18.2k - 37k"};
    Bracket brac4{C(18200), C(37000)};
    FnCalc fn4 = FN_CALC_SIGNATURE {
        CashAmount taxable = brac4.in_bracket(slice);
        CashAmount payable = taxable * 0.19;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule std_bracket2{102, slug4, desc4, fn4};

    // Bracket Rule 5

    std::string slug5{"Bracket 37k - 90k"};
    std::string desc5{"32.5% on income 37k - 90k"};
    Bracket brac5{C(37000), C(90000)};
    FnCalc fn5 = FN_CALC_SIGNATURE {
        CashAmount taxable = brac5.in_bracket(slice);
        CashAmount payable = taxable * 0.325;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule std_bracket3{103, slug5, desc5, fn5};

    // Bracket Rule 6

    std::string slug6{"Bracket 90k - 180k"};
    std::string desc6{"37% on income 90k - 180k"};
    Bracket brac6{C(90000), C(180000)};
    FnCalc fn6 = FN_CALC_SIGNATURE {
        CashAmount taxable = brac6.in_bracket(slice);
        CashAmount payable = taxable * 0.37;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule std_bracket4{104, slug6, desc6, fn6};

    // Bracket Rule 7

    std::string slug7{"Bracket 180k - inf"};
    std::string desc7{"45% on income over 180k"};
    Bracket brac7{C(180000), C(999999999)};
    FnCalc fn7 = FN_CALC_SIGNATURE {
        CashAmount taxable = brac7.in_bracket(slice);
        CashAmount payable = taxable * 0.45;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule std_bracket5{105, slug7, desc7, fn7};

    // LIT Bracket Rule 8

    std::string slug8{"Low-income-tax offset <67k"};
    std::string desc8{
        "Maximum offset of 445 applies below 37k, then phases out"};
    Bracket brac8{C(0), C(37000)};
    FnCalc fn8 = FN_CALC_SIGNATURE {
        CashAmount taxable = brac8.in_bracket(slice);
        double proportion = taxable / (brac8.upper() - brac8.lower());
        CashAmount payable = C(445) * proportion;
        LineItem item{taxable, payable, CreditDebit::CREDIT};
        return {item};
    };

    Rule lit_bracket1{201, slug8, desc8, fn8};

    // LIT Bracket Rule 9

    std::string slug9{"Low-income-tax offset <67k"};
    std::string desc9{
        "Maximum offset of 445 applies below 37k, then phases out"};
    Bracket brac9{C(37000), C(66667)};
    FnCalc fn9 = FN_CALC_SIGNATURE {
        CashAmount total_income = taxret.total_income();
        CashAmount threshold_phaseout = C(37000);
        CashAmount step{150L};

        if (total_income <= brac9.upper()) {
            CashAmount taxable = brac8.in_bracket(slice);
            CashAmount payable = C(445);

            if (total_income > threshold_phaseout) {
                CashAmount limit = std::min(total_income, brac8.upper());
                CashAmount range = limit - threshold_phaseout;
                CashAmount subtract = std::min(payable, range.dollars() * step);
                payable = payable - subtract;
            }

            LineItem item{taxable, payable, CreditDebit::CREDIT};
            return {item};
        }

        return {};
    };

    Rule lit_bracket2{202, slug9, desc9, fn9};

    auto rules = {std_bracket1, std_bracket2,
                  std_bracket3, std_bracket4,
                  std_bracket5, /* lit_bracket1, */ medicare_levy};
    RuleSet ruleset{"aus-2020", "Australian income tax 2020", rules};
    return ruleset;
}

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
    RuleSet ruleset = build_ruleset();

    TaxCalculation calc = ruleset.apply(taxret);
    auto slices = calc.slices();

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
            const Rule &rule = calc.get_rule(rule_items.rule_id());

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
        const Rule &rule = calc.get_rule(rule_items.rule_id());

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