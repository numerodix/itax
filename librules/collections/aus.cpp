#include "aus.h"

namespace rules {
namespace collections {

using core::Bracket;
using core::CashAmount;
using core::CreditDebit;
using core::FnCalc;
using core::IncomeSlice;
using core::LineItem;
using core::TaxReturn;

/////////////////////////////////////////////////////////////////////////////
// Financial year 2019
/////////////////////////////////////////////////////////////////////////////

Rule get_aus_rev_fy19_bracket1() {
    std::string slug{"Bracket 0 - 18.2k"};
    std::string desc{"0% on income 0k - 18.2k"};

    Bracket bracket{C(0), C(18200)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.0;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_BRACKET1, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy19_bracket2() {
    std::string slug{"Bracket 18.2k - 37k"};
    std::string desc{"19% on income 18.2k - 37k"};

    Bracket bracket{C(18200), C(37000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.19;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_BRACKET2, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy19_bracket3() {
    std::string slug{"Bracket 37k - 90k"};
    std::string desc{"32.5% on income 37k - 90k"};

    Bracket bracket{C(37000), C(90000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.325;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_BRACKET3, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy19_bracket4() {
    std::string slug{"Bracket 90k - 180k"};
    std::string desc{"37% on income 90k - 180k"};

    Bracket bracket{C(90000), C(180000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.37;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_BRACKET4, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy19_bracket5() {
    std::string slug{"Bracket 180k - inf"};
    std::string desc{"45% on income over 180k"};

    Bracket bracket{C(180000), CashAmount::max()};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.45;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_BRACKET5, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy19_medicare_levy() {
    std::string slug{"Medicare levy"};
    std::string desc{"Medicare levy is 2% on all income"};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = slice.amount();
        CashAmount payable = taxable * 0.02;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_MEDICARE_LEVY, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy19_medicare_levy_surcharge() {
    std::string slug{"Medicare levy surcharge"};
    std::string desc{"Medicare levy surcharge is 1.5% on all income if total "
                     "income is over 90k"};

    FnCalc fn = FN_CALC_SIGNATURE {
        if (taxret.total_income() > C(90000)) {
            CashAmount taxable = slice.amount();
            CashAmount payable = taxable * 0.015;

            if (payable > C(0)) {
                LineItem item{taxable, payable, CreditDebit::DEBIT};
                return {item};
            }
        }

        return {};
    };

    Rule rule{AUS_REV_FY19_MEDICARE_LEVY_SURCHARGE, slug, desc, fn};
    return rule;
}

} // namespace collections
} // namespace rules