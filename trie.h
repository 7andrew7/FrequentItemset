#pragma once

#include <string>
#include <unordered_map>

#include "timing.h"

class TrieNode {
    using item_t = int32_t;

public:

    TrieNode() : TrieNode{-1} {}
    TrieNode(item_t item) : TrieNode{item, 0, 0} {}
    TrieNode(item_t item, int32_t height, std::size_t count) :
        _item{item},
        _height{height},
        _count{count} {}

    ~TrieNode() {
        for (const auto &kv : _child_ptr_map) {
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
        assert (k > 0);

        if (std::distance(begin, end) < k)
            return; // not enough input items to consturct a k-combination

        if (_height + 1 < k)
            return; // not enough trie height to construct a k-combination

        if (k == 1) {
            // acquire mutex
            for (auto it = begin; it != end; ++it) {
                auto map_it = _child_ptr_map.find(*it);
                if (map_it != _child_ptr_map.end()) {
                    auto child_ptr = map_it->second;
                    child_ptr->_count++;
                } else {
                    auto child_ptr = new TrieNode{*it, 0, 1};
                    _child_ptr_map.emplace(*it, child_ptr);
                }
            }
        } else {
            // Include the first element; construct k-1 combinations from the remainder
            auto it = _child_ptr_map.find(*begin);
            if (it != _child_ptr_map.end()) {
                auto child_ptr = it->second;
                child_ptr->increment_combinations(begin + 1, end, k - 1);
            } else {
                // don't generate new nodes for k > 1 (due to monotonicity)
            }

            // skip the first element; construct k-combinations from the remainder
            increment_combinations(begin + 1, end, k);
        }
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
        int32_t new_height{};

        if (depth > 1) {
            for (auto &kv_pair : _child_ptr_map) {
                auto const child_ptr = kv_pair.second;
                remaining += child_ptr->prune_candidates(support, depth - 1);
                new_height = std::max(new_height, child_ptr->_height + 1);
            }
        } else {
            // delete children with insufficient support
            auto it = _child_ptr_map.begin();
            while (it != _child_ptr_map.end()) {
                assert(it->second);
                if (it->second->_count < support) {
                    auto to_delete = it;
                    ++it;
                    _child_ptr_map.erase(to_delete);
                } else {
                    ++it;
                    ++remaining;
                    new_height = 1;
                }
            }
        }

        _height = new_height;
        return remaining;
    }


    void debug_print(std::ostream &out, int indent=0) const
    {
        std::cout << std::string(indent, ' ') << *this << std::endl;
        for (auto &kv_pair : _child_ptr_map) {
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
        for (auto &kv_pair : _child_ptr_map) {
            auto item = kv_pair.first;
            auto node = kv_pair.second;
            buffer->push_back(item);
            node->export_recursive(buffer, output);
            buffer->pop_back();
        }
    }

    item_t _item;
    int32_t _height; // Length of a longest path to a leaf
    std::size_t _count;

    std::unordered_map<item_t, TrieNode *> _child_ptr_map;
};

std::ostream &operator<<(std::ostream &out, const TrieNode &node) {

    out << node._item << ": " << node._count << " [";
    for (auto kv_pair : node._child_ptr_map) {
        out << kv_pair.first << " ";
    }
    out << "]";

    return out;
}
