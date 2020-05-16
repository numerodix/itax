#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "libcore/argparse.h"
#include "libcore/bracket.h"
#include "libcore/rule.h"
#include "libcore/shortcuts.h"

using namespace core;
using namespace std;

std::vector<Rule> build_rules() {

    // Rule 1

    std::string slug1{"Medicare levy"};
    std::string desc1{"Medicare levy is 2% on all income"};
    FnCalc fn1 = FN_CALC_SIGNATURE {
        CashAmount taxable = slice.amount();
        CashAmount payable = taxable * 0.02;
        LineItem item{taxable, payable, CreditDebit::DEBIT};
        return {item};
    };

    Rule rule1{1, slug1, desc1, fn1};

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

    Rule rule2{2, slug2, desc2, fn2};

    // Bracket Rule 3

    std::string slug3{"bracket 0 - 18.2k"};
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

    Rule rule3{3, slug3, desc3, fn3};

    // Bracket Rule 4

    std::string slug4{"bracket 18.2k - 37k"};
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

    Rule rule4{4, slug4, desc4, fn4};

    // Bracket Rule 5

    std::string slug5{"bracket 37k - 90k"};
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

    Rule rule5{5, slug5, desc5, fn5};

    // Bracket Rule 6

    std::string slug6{"bracket 90k - 180k"};
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

    Rule rule6{6, slug6, desc6, fn6};

    // Bracket Rule 7

    std::string slug7{"bracket 180k - inf"};
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

    Rule rule7{7, slug7, desc7, fn7};

    // LIT Bracket Rule 8

    std::string slug8{"low-income-tax offset <67k"};
    std::string desc8{
        "Maximum offset of 445 applies below 37k, then phases out"};
    Bracket brac8{C(0), C(66667)};
    FnCalc fn8 = FN_CALC_SIGNATURE {
        CashAmount total_income = taxret.total_income();
        CashAmount threshold_phaseout = C(37000);
        CashAmount step{150L};

        if (total_income <= brac8.upper()) {
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

    Rule rule8{8, slug8, desc8, fn8};

    // return {rule1, rule2, rule3, rule4, rule5, rule6, rule7};
    return {rule1, rule3, rule4, rule5, rule6, rule7, rule8};
}

std::string fmt(CreditDebit cd) {
    return cd == CreditDebit::CREDIT ? " " : "-";
}

std::string join(std::string prefix, std::string number) {
    stringstream ss{};
    ss << prefix << number;
    return ss.str();
}

std::ostream &numfmt(std::ostream &out) {
    out << right << setfill(' ') << setw(12);
    return out;
}

int main(int argc, char *argv[]) {
    ArgParser parser{};
    TaxReturn taxret = parser.parse(argc, argv)[0];
    auto rules = build_rules();

    int slice_no = 0;
    LineItem total{};

    for (const IncomeSlice &slice : taxret.slices()) {
        cout << "Slice " << ++slice_no
             << " :: " << slice.amount().display_with_commas() << "  [ "
             << slice.lower_bound().display_with_commas() << " - "
             << slice.upper_bound().display_with_commas() << " ]" << '\n';

        LineItem slice_total{};
        for (const Rule &rule : rules) {
            auto res = rule.calculate(taxret, slice);
            slice_total = slice_total + res.net();

            if (res.net().payable() == C(0)) {
                continue;
            }

            std::string prefix = fmt(res.net().credit_debit());
            std::string number =
                join(prefix, res.net().payable().display_with_commas());
            cout << "         " << numfmt << number;

            cout << "\t[" << rule.slug() << "] " << '\n';
        }

        std::string prefix = fmt(slice_total.credit_debit());
        std::string number =
            join(prefix, slice_total.payable().display_with_commas());
        cout << "  Total: " << numfmt << number << '\n';
        cout << '\n';

        total = total + slice_total;
    }

    std::string prefix = fmt(total.credit_debit());
    std::string tax = join(prefix, total.payable().display_with_commas());

    CashAmount after_tax = taxret.total_income() - total.payable();

    cout << "Total income: " << numfmt
         << taxret.total_income().display_with_commas() << '\n';
    cout << "Total tax   : " << numfmt << tax << '\n';
    cout << "After tax   : " << numfmt << after_tax.display_with_commas()
         << '\n';
}