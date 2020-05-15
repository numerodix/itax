#ifndef LIBCORE_RULE_ITEMS_H
#define LIBCORE_RULE_ITEMS_H

#include <vector>

#include "defs.h"
#include "line_item.h"

namespace core {

class RuleItems {
  public:
    RuleItems(RuleId rule_id, std::vector<LineItem> items);
    ~RuleItems();

    RuleItems(const RuleItems &other) = delete;
    RuleItems &operator=(const RuleItems &other) = delete;

    RuleItems(RuleItems &&other) = delete;
    RuleItems &operator=(RuleItems &&other) = delete;

    RuleId rule_id() const;
    const std::vector<LineItem> &items() const;

    LineItem net() const;

  private:
    RuleId m_rule_id;
    std::vector<LineItem> m_items;
};

} // namespace core

#endif // LIBCORE_RULE_ITEMS_H
