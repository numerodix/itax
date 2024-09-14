#include "aus.h"

#include "libcore/bracket.h"
#include "libcore/cash_amount.h"
#include "libcore/format.h"
#include "libcore/shortcuts.h"

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
// Financial year 2025
// preliminary source:
// https://www.smh.com.au/politics/federal/tax-cuts-to-flow-within-weeks-after-ato-receives-assurance-of-labor-s-support-20201007-p562vx.html
/////////////////////////////////////////////////////////////////////////////

Rule get_aus_rev_fy25_bracket1() {
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

    Rule rule{AUS_REV_FY25_BRACKET1, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy25_bracket2() {
    std::string slug{"Bracket 18.2k - 45k"};
    std::string desc{"16% on income 18.2k - 45k"};

    Bracket bracket{C(18200), C(45000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.16;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY25_BRACKET2, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy25_bracket3() {
    std::string slug{"Bracket 45k - 135k"};
    std::string desc{"30% on income 45k - 135k"};

    Bracket bracket{C(45000), C(135000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.3;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY25_BRACKET3, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy25_bracket4() {
    std::string slug{"Bracket 135k - 190k"};
    std::string desc{"37% on income 135k - 190k"};

    Bracket bracket{C(135000), C(190000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.37;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY25_BRACKET4, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy25_bracket5() {
    std::string slug{"Bracket 190k - inf"};
    std::string desc{"45% on income over 190k"};

    Bracket bracket{C(190000), CashAmount::max()};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.45;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY25_BRACKET5, slug, desc, fn};
    return rule;
}

/////////////////////////////////////////////////////////////////////////////
// Financial year 2021
/////////////////////////////////////////////////////////////////////////////

Rule get_aus_rev_fy21_bracket1() {
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

    Rule rule{AUS_REV_FY21_BRACKET1, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy21_bracket2() {
    std::string slug{"Bracket 18.2k - 45k"};
    std::string desc{"19% on income 18.2k - 45k"};

    Bracket bracket{C(18200), C(45000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.19;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY21_BRACKET2, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy21_bracket3() {
    std::string slug{"Bracket 45k - 120k"};
    std::string desc{"32.5% on income 45k - 120k"};

    Bracket bracket{C(45000), C(120000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.325;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY21_BRACKET3, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy21_bracket4() {
    std::string slug{"Bracket 120k - 180k"};
    std::string desc{"37% on income 120k - 180k"};

    Bracket bracket{C(120000), C(180000)};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.37;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY21_BRACKET4, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy21_bracket5() {
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

    Rule rule{AUS_REV_FY21_BRACKET5, slug, desc, fn};
    return rule;
}

Rule get_aus_rev_fy21_lito() {
    std::string slug{"Low Income Tax Offset 0 - 66.7k"};
    std::string desc{"Low Income Tax Offset is up to 700 when "
                     "income is up to 66.7k"};

    Bracket first{C(0), C(37500)};
    Bracket second{C(37500), C(45000)};
    Bracket third{C(45000), C(66667)};
    CashAmount offset = C(700);

    FnCalc fn = FN_CALC_SIGNATURE {
        // first bracket provides the whole offset
        CashAmount taxable_fst = first.in_bracket(slice);
        CashAmount base = std::min(first.upper(), taxret.total_income());
        double proportion_fst = taxable_fst / base;
        CashAmount payable_fst = offset * proportion_fst;
        LineItem item_fst{taxable_fst, payable_fst, CreditDebit::CREDIT};

        // second bracket is a partial phase out at 5 cents / dollar
        CashAmount range_snd = second.range();
        CashAmount taxable_snd = second.in_bracket(slice);
        double proportion_snd = taxable_snd / range_snd;
        CashAmount payable_snd = range_snd * 0.05 * proportion_snd;
        LineItem item_snd{taxable_snd, payable_snd, CreditDebit::DEBIT};

        // third bracket is a phase out at 1.5 cents / dollar, to zero
        CashAmount range_thd = third.range();
        CashAmount taxable_thd = third.in_bracket(slice);
        double proportion_thd = taxable_thd / range_thd;
        CashAmount payable_thd = range_thd * 0.015 * proportion_thd;

        // Hack: the 0.015 multiplier above tends to create a 50 in the
        // rounding_part which is rounded up to one cent in the overall
        // calculation and fails to precisely cancel out the amounts from the
        // first and second brackets. This is all because we are multiplying
        // 0.015 by a number ending in 667, as per the bracket upper bound.
        // Here we detect whether combining the three parts produces a quantity
        // that is less than one cent. If it is, then we discard it.
        if ((payable_fst.raw() - payable_snd.raw() - payable_thd.raw()) < 100L) {
            payable_thd.zero_rounding_part();
        }

        LineItem item_thd{taxable_thd, payable_thd, CreditDebit::DEBIT};

        return {item_fst, item_snd, item_thd};
    };

    Rule rule{AUS_REV_FY21_LITO, slug, desc, fn};
    return rule;
}

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

Rule get_aus_rev_fy19_lmito() {
    std::string slug{"Low/Middle Income Tax Offset 0 - 126k"};
    std::string desc{"Low and Middle Income Tax Offset is up to 1080 when "
                     "income is up to 126k"};

    Bracket first{C(0), C(37000)};
    Bracket second{C(37000), C(48000)};
    Bracket third{C(48000), C(90000)};
    Bracket fourth{C(90000), C(126000)};
    CashAmount offset = C(255);

    FnCalc fn = FN_CALC_SIGNATURE {
        // first bracket
        CashAmount taxable_fst = first.in_bracket(slice);
        CashAmount base = std::min(first.upper(), taxret.total_income());
        double proportion_fst = taxable_fst / base;
        CashAmount payable_fst = offset * proportion_fst;
        LineItem item_fst{taxable_fst, payable_fst, CreditDebit::CREDIT};

        // second bracket
        CashAmount range_snd = second.range();
        CashAmount taxable_snd = second.in_bracket(slice);
        double proportion_snd = taxable_snd / range_snd;
        CashAmount payable_snd = range_snd * 0.075 * proportion_snd;
        LineItem item_snd{taxable_snd, payable_snd, CreditDebit::CREDIT};

        // third bracket adds no credit nor debit
        CashAmount taxable_thd = third.in_bracket(slice);
        CashAmount payable_thd = C(0);
        LineItem item_thd{taxable_thd, payable_thd, CreditDebit::CREDIT};

        // fourth bracket
        CashAmount range_fth = fourth.range();
        CashAmount taxable_fth = fourth.in_bracket(slice);
        double proportion_fth = taxable_fth / range_fth;
        CashAmount payable_fth = range_fth * 0.03 * proportion_fth;
        LineItem item_fth{taxable_fth, payable_fth, CreditDebit::DEBIT};

        return {item_fst, item_snd, item_thd, item_fth};
    };

    Rule rule{AUS_REV_FY19_LMITO, slug, desc, fn};
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

/////////////////////////////////////////////////////////////////////////////
// Financial year 2013
/////////////////////////////////////////////////////////////////////////////

Rule get_aus_rev_fy13_lito() {
    std::string slug{"Low Income Tax Offset 0 - 67k"};
    std::string desc{
        "Low Income Tax Offset is up to 445 when income is up to 66667"};

    Bracket constant{C(0), C(37000)};
    Bracket phaseout{C(37000), C(66667)};
    CashAmount decrement{150L};
    CashAmount offset = C(445);

    FnCalc fn = FN_CALC_SIGNATURE {
        // constant bracket
        CashAmount taxable_con = constant.in_bracket(slice);
        CashAmount base = std::min(constant.upper(), taxret.total_income());
        double proportion_con = taxable_con / base;
        CashAmount payable_con = offset * proportion_con;
        LineItem item_constant{taxable_con, payable_con, CreditDebit::CREDIT};

        // phaseout bracket
        CashAmount taxable_ph = phaseout.in_bracket(slice);
        CashAmount deduction =
            std::min(offset, decrement * phaseout.range().dollars());
        double proportion_ph = taxable_ph / phaseout.range();
        CashAmount payable_ph = std::min(offset, deduction * proportion_ph);
        LineItem item_phaseout{taxable_ph, payable_ph, CreditDebit::DEBIT};

        return {item_constant, item_phaseout};
    };

    Rule rule{AUS_REV_FY13_LITO, slug, desc, fn};
    return rule;
}

// NOTE: Takes into account only income, not super contributions.
Rule get_aus_rev_fy13_division_293() {
    std::string slug{"Division 293"};
    std::string desc{"Division 293 is 15% on income above 250k"};

    Bracket bracket{C(250000), CashAmount::max()};

    FnCalc fn = FN_CALC_SIGNATURE {
        CashAmount taxable = bracket.in_bracket(slice);
        CashAmount payable = taxable * 0.15;

        if (payable > C(0)) {
            LineItem item{taxable, payable, CreditDebit::DEBIT};
            return {item};
        }

        return {};
    };

    Rule rule{AUS_REV_FY13_DIVISION_293, slug, desc, fn};
    return rule;
}

} // namespace collections
} // namespace rules