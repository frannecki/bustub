#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  // throw NotImplementedException("Trie::Get is not implemented.");

  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
  auto cur = GetTrieNode(key);
  auto cur_with_value = dynamic_cast<const TrieNodeWithValue<T>*>(cur.get());
  if (cur_with_value) {
    return cur_with_value->value_.get();
  }
  return nullptr;
}

auto Trie::GetTrieNode(std::string_view key) const -> std::shared_ptr<const TrieNode> {
  auto cur = root_;
  for (char ch : key) {
    if (!cur) break;
    auto it = cur->children_.find(ch);
    if (it == cur->children_.end()) return nullptr;
    cur = it->second;
  }
  return cur;
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  // throw NotImplementedException("Trie::Put is not implemented.");

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
  if (key.empty()) return Trie(root_);

  std::shared_ptr<TrieNode> prev{nullptr};
  std::shared_ptr<const TrieNode> root{nullptr};
  if (root_) {
    root = std::shared_ptr<const TrieNode>(std::move(root_->Clone()));
  } else {
    root = std::make_shared<const TrieNode>();
  }
  auto cur = root;

  for (size_t i = 0; i < key.size(); ++i) {
    prev = std::const_pointer_cast<TrieNode>(cur);
    auto it = prev->children_.find(key[i]);
    if (i+1 == key.size()) {
      cur = std::shared_ptr<const TrieNode>(new TrieNodeWithValue<T>(
        std::make_shared<T>(std::move(value))));
    } else if (it != prev->children_.end()) {
      cur = std::shared_ptr<const TrieNode>(std::move(it->second->Clone()));
    } else {
      cur = std::make_shared<const TrieNode>();
    }

    if (it != prev->children_.end()) {
      it->second = cur;
    } else {
      prev->children_.insert(std::pair<char, std::shared_ptr<const TrieNode>>(
          key[i], cur));
    }
  }
  
  return Trie(root);
}

auto Trie::Remove(std::string_view key) const -> Trie {
  // throw NotImplementedException("Trie::Remove is not implemented.");

  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
  if (key.empty() || !root_ || !GetTrieNode(key)) return Trie(root_);

  std::shared_ptr<TrieNode> prev{nullptr};  
  auto root = std::shared_ptr<const TrieNode>(std::move(root_->Clone()));
  auto cur = root;

  for (size_t i = 0; i < key.size(); ++i) {
    prev = std::const_pointer_cast<TrieNode>(cur);
    auto it = prev->children_.find(key[i]);
    if (i+1 == key.size()) {
      // wipe out value
      cur = std::make_shared<const TrieNode>(it->second->children_);
      if (!cur->children_.empty()) {
        it->second = cur;
      }
    } else {
      cur = std::shared_ptr<const TrieNode>(std::move(it->second->Clone()));
      it->second = cur;
    }
  }

  return Trie(root);
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
