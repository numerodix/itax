#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "libcore/argparse.h"
#include "libcore/rule.h"
#include "libcore/shortcuts.h"

using namespace core;
using namespace std;

CashAmount get_in_bracket(CashAmount lower, CashAmount upper,
                          const IncomeSlice &slice) {
    // the slice is entirely below or above the bracket
    if ((slice.upper_bound() < lower) || (upper < slice.lower_bound())) {
        return CashAmount{0L};
    }

    // the slice is entirely contained between the bounds of the bracket
    if ((lower < slice.lower_bound()) && (slice.upper_bound() < upper)) {
        return slice.amount();
    }

    // the slice overlaps or exceeds the whole bracket
    if ((slice.lower_bound() <= lower) && (upper <= slice.upper_bound())) {
        return upper - lower;
    }

    // the slice begins before the bracket and ends in the bracket
    if ((slice.lower_bound() < lower) && (slice.upper_bound() < upper)) {
        return slice.upper_bound() - lower;
    }

    // the slice begins inside the bracket and ends after the bracket
    if ((lower < slice.lower_bound()) && (upper < slice.upper_bound())) {
        return upper - slice.lower_bound();
    }

    throw runtime_error("should not reach this point");
}

std::vector<Rule> build_rules() {

    // Rule 1

    std::string slug1{"Medicare levy"};
    std::string desc1{"Medicare levy is 2% on all income"};
    FnCalcForSlice fn1 = FN_CALC_SLICE_SIG {
        CashAmount taxable = slice.amount();
        CashAmount payable = taxable * 0.02;
        LineItem item{taxable, payable, CreditDebit::DEBIT};
        return {item};
    };

    Rule rule1{1, slug1, desc1, fn1};

    // Rule 2

    std::string slug2{"Medicare levy surcharge"};
    std::string desc2{
        "Medicare levy surcharge is 1.5% on all income if total income above 90k"};
    FnCalcForSlice fn2 = FN_CALC_SLICE_SIG {
        if (context.tax_return().total_income() > C(90000)) {
            CashAmount taxable = slice.amount();
            CashAmount payable = taxable * 0.015;
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }
        return {};
    };

    Rule rule2{2, slug2, desc2, fn2};

    // Rule 3

    std::string slug3{"bracket 0 - 18.2k"};
    std::string desc3{"0% on income 0k - 18.2k"};
    FnCalcForSlice fn3 = FN_CALC_SLICE_SIG { return {}; };

    Rule rule3{3, slug3, desc3, fn3};

    // Rule 4

    std::string slug4{"bracket 18.2k - 37k"};
    std::string desc4{"19% on income 18.2k - 37k"};
    FnCalcForSlice fn4 = FN_CALC_SLICE_SIG {
        CashAmount in_bracket = get_in_bracket(C(18200), C(37000), slice);
        if (in_bracket > C(0)) {
            CashAmount taxable = in_bracket;
            CashAmount payable = taxable * 0.19;
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }
        return {};
    };

    Rule rule4{4, slug4, desc4, fn4};

    // Rule 5

    std::string slug5{"bracket 37k - 90k"};
    std::string desc5{"32.5% on income 37k - 90k"};
    FnCalcForSlice fn5 = FN_CALC_SLICE_SIG {
        CashAmount in_bracket = get_in_bracket(C(37000), C(90000), slice);
        if (in_bracket > C(0)) {
            CashAmount taxable = in_bracket;
            CashAmount payable = taxable * 0.325;
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }
        return {};
    };

    Rule rule5{5, slug5, desc5, fn5};

    // Rule 6

    std::string slug6{"bracket 90k - 180k"};
    std::string desc6{"37% on income 90k - 180k"};
    FnCalcForSlice fn6 = FN_CALC_SLICE_SIG {
        CashAmount in_bracket = get_in_bracket(C(90000), C(180000), slice);
        if (in_bracket > C(0)) {
            CashAmount taxable = in_bracket;
            CashAmount payable = taxable * 0.37;
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }
        return {};
    };

    Rule rule6{6, slug6, desc6, fn6};

    // Rule 7

    std::string slug7{"bracket 180k - inf"};
    std::string desc7{"45% on income over 180k"};
    FnCalcForSlice fn7 = FN_CALC_SLICE_SIG {
        CashAmount in_bracket = get_in_bracket(C(180000), C(9999999), slice);
        if (in_bracket > C(0)) {
            CashAmount taxable = in_bracket;
            CashAmount payable = taxable * 0.45;
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }
        return {};
    };

    Rule rule7{7, slug7, desc7, fn7};

    // return {{rule1, rule2, rule3, rule4, rule5, rule6, rule7}};
    return {{rule1, rule3, rule4, rule5, rule6, rule7}};
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
    Context context{taxret};
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
            auto res = rule.calculate(context, slice);
            slice_total = slice_total + res.net();

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

    CashAmount after_tax = context.tax_return().total_income() - total.payable();

    cout << "Total income: " << numfmt
         << context.tax_return().total_income().display_with_commas() << '\n';
    cout << "Total tax   : " << numfmt << tax << '\n';
    cout << "After tax   : " << numfmt << after_tax.display_with_commas()
         << '\n';
}