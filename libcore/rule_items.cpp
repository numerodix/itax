#include "rule_items.h"

#include "algos.h"

namespace core {

RuleItems::RuleItems(RuleId rule_id, std::vector<LineItem> items)
    : m_rule_id{rule_id}, m_items{items} {}

RuleItems::RuleItems() : m_rule_id{0}, m_items{{}} {}

RuleItems::~RuleItems() = default;

RuleItems::RuleItems(const RuleItems &other) = default;

RuleItems &RuleItems::operator=(const RuleItems &other) = default;

RuleItems::RuleItems(RuleItems &&other) = default;

RuleItems &RuleItems::operator=(RuleItems &&other) = default;

RuleId RuleItems::rule_id() const { return m_rule_id; }

const std::vector<LineItem> &RuleItems::items() const { return m_items; }

LineItem RuleItems::net() const {
    return sum(m_items.begin(), m_items.end(), LineItem{});
}

void RuleItems::append_item(const LineItem &item) { m_items.push_back(item); }

} // namespace core