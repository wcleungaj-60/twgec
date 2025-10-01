#include "ast.h"
#include "keyword.h"
#include "transform.h"
namespace transform {

using std::string;
using std::unique_ptr;

bool implicitListPromotion(const unique_ptr<ModuleNode> &moduleNode) {
  for (auto &metadata : moduleNode->metadatas) {
    if ((metadata->key == keyword::config::royalLocs ||
         metadata->key == keyword::config::skydowLocs ||
         metadata->key == keyword::config::thirdLocs ||
         metadata->key == keyword::config::customWeapons) &&
        !dynamic_cast<ListValueNode *>(metadata->expNode->value.get())) {
      auto listNode =
          std::make_unique<ListValueNode>(metadata->expNode->value->loc);
      listNode->items.push_back(metadata->expNode->clone());
      metadata->expNode->value = std::move(listNode);
    }
  }
  return true;
}
} // namespace transform