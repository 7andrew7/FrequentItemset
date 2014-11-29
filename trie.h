#pragma once

#include <string>
#include <unordered_map>

#include "timing.h"

class TrieNode {
    using item_t = int32_t;
    using MapType = std::unordered_map<item_t, TrieNode *>;

public:

    TrieNode() : TrieNode{-1} {}
    TrieNode(item_t item) : TrieNode{item, 0, 0} {}
    TrieNode(item_t item, int32_t max_height, std::size_t count) :
        _item{item},
        _max_height{max_height},
        _count{count} {}

    ~TrieNode() {
        for (const auto &kv : _map) {
            delete kv.second;
        }
    }

    /**
     * Increment all existing combinations of [begin, end) of size k.
     */
    template<class InputIterator>
    void increment_combinations(
        InputIterator begin,
        InputIterator end,
        int32_t k)
    {
        if (k == 0) {
            _count++;
            return;
        }

        if (std::distance(begin, end) < k)
            return; // not enough items left

        if (_max_height + 1 < k)
            return; // not enough trie depth

        // consider adding the first character to the combination
        auto it = _map.find(*begin);
        if (it != _map.end()) {
            auto child_ptr = it->second;
            assert (child_ptr != nullptr);
            child_ptr->increment_combinations(begin + 1, end, k - 1);
            _max_height = std::max(_max_height, child_ptr->_max_height + 1);
        } else if (k == 1) {
            // Generate a new leaf
            auto child_ptr = new TrieNode{*begin, 0, 1};
            _map.emplace(*begin, child_ptr);
            _max_height = 1;
        } else {
            // don't generate new nodes for k > 1 (due to monotonicity)
        }

        // skip the first element; construct combinations from the remainder
        increment_combinations(begin + 1, end, k);
    }

    /**
     * Remove trie nodes at the given depth with insufficient support.
     *
     * Returns the sum of candidates that were *not* pruned.
     */
    std::size_t prune_candidates(
        std::size_t support,
        std::size_t depth)
    {
        std::size_t remaining{};
        int32_t new_max_height{};

        if (depth > 1) {
            for (auto &kv_pair : _map) {
                auto const child_ptr = kv_pair.second;
                remaining += child_ptr->prune_candidates(support, depth - 1);
                new_max_height = std::max(new_max_height, child_ptr->_max_height + 1);
            }
        } else {
            // delete children with insufficient support
            auto it = _map.begin();
            while (it != _map.end()) {
                assert(it->second);
                if (it->second->_count < support) {
                    auto to_delete = it;
                    ++it;
                    _map.erase(to_delete);
                } else {
                    ++it;
                    ++remaining;
                    new_max_height = 1;
                }
            }
        }

        _max_height = new_max_height;
        return remaining;
    }


    void debug_print(std::ostream &out, int indent=0) const
    {
        std::cout << std::string(indent, ' ') << *this << std::endl;
        for (auto &kv_pair : _map) {
            kv_pair.second->debug_print(out, indent + 2);
        }
    }

    template<class BasketType>
    void export_output(BasketType *output) const
    {
        std::vector<item_t> buffer{};
        export_recursive(&buffer, output);
    }

    friend std::ostream &operator<<(std::ostream &out, const TrieNode &node);

private:

    template<class BasketType>
    void export_recursive(
        std::vector<item_t> *buffer,
        BasketType *output) const
    {
        if (buffer->size() > 0)
            output->add_basket(*buffer);
        for (auto &kv_pair : _map) {
            auto item = kv_pair.first;
            auto node = kv_pair.second;
            buffer->push_back(item);
            node->export_recursive(buffer, output);
            buffer->pop_back();
        }
    }

    item_t _item;
    int32_t _max_height; // largest child height + 1
    std::size_t _count;
    MapType _map;
};

std::ostream &operator<<(std::ostream &out, const TrieNode &node) {

    out << node._item << ": " << node._count << " [";
    for (auto kv_pair : node._map) {
        out << kv_pair.first << " ";
    }
    out << "]";

    return out;
}
