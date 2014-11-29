#pragma once

#include <string>
#include <unordered_map>

#include "timing.h"

class TrieNode {
    using item_t = int32_t;
    using MapType = std::unordered_map<item_t, TrieNode *>;

public:

    TrieNode() : TrieNode{-1} {}
    TrieNode(item_t item) : TrieNode{item, 0} {}
    TrieNode(item_t item, int32_t max_height) : 
        _item{item},
        _max_height{max_height},
        _count{} { }

    ~TrieNode() {
        for (const auto &kv : _map) {
            delete kv.second;
        }
    }
    /**
     * Lookup or insert a singleton basket to the trie; increase its count.
     */
    void increment_singleton_count(item_t item)
    {
        auto child = lookup_or_create(item);
        assert(child);
        child->_count++;
    }

    /**
     * Increment all existing combinations of [begin, end) of size k.
     *
     * Non-existent combinations are silently dropped.
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

        auto dst = std::distance(begin, end);
        if (dst < k)
            return; // not enough items left

        if (_max_height < k)
            return; // not enough trie depth

        // consider adding the first character to the combination
        auto it = _map.find(*begin);
        if (it != _map.end()) {
            auto child = it->second;
            assert (child != nullptr);
            child->increment_combinations(begin + 1, end, k - 1);
        }

        // skip the first element; construct combinations from the remainder
        increment_combinations(begin + 1, end, k);
    }

    /**
     * Generate candidates at a given depth; assumes that frequent items of
     * size k-1 have already been computed.
    */
    void candidate_gen(int32_t remaining_hops)
    {
        if ((_max_height + 1) < remaining_hops)
            return;

        if (remaining_hops == 2) {
            for (auto it1 = _map.begin(); it1 != _map.end(); ++it1) {
                auto it2 = it1;
                for (++it2; it2 != _map.end(); ++it2) {
                    auto min_max_pair = std::minmax(it1->first, it2->first);
                    auto lesser_child_node = _map[min_max_pair.first];
                    assert (lesser_child_node != nullptr);
                    lesser_child_node->create(min_max_pair.second);
                }
            }
            _max_height = 2;
        } else {
             for (auto &kv_pair : _map) {
                kv_pair.second->candidate_gen(remaining_hops - 1);
                _max_height = std::max(_max_height, kv_pair.second->_max_height + 1);
            }
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

        if (depth > 1) {
            for (auto &kv_pair : _map)
                remaining += kv_pair.second->prune_candidates(support, depth - 1);
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
                }
            }
        }

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

    TrieNode *create(item_t item)
    {
        auto &child = _map[item];
        assert (child == nullptr);
        child = new TrieNode{item};
        _max_height = 1; // assumes Trie is grown breadth-first
        return child;
    }

    TrieNode *lookup_or_create(item_t item)
    {
        auto &child = _map[item];
        if (child == nullptr)            
            child = new TrieNode{item};
        _max_height = 1; // assumes Trie is grown breadth-first
        return child;
    }

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
