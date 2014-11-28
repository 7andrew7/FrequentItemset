#pragma once

#include <string>
#include <unordered_map>

class TrieNode {
    using item_t = int32_t;
    using MapType = std::unordered_map<item_t, TrieNode *>;

public:

    TrieNode() : _item{-1}, _count{} { }
    TrieNode(item_t item) : _item{item}, _count{} { }

    /**
     * Add a singleton basket to the trie; increase its count.
     */
    void increment_singleton_count(item_t item)
    {
        auto child = lookup_or_create(item);
        assert(child);
        child->_count++;
    }

    /**
     * Increment the count of an existing Trie element; has no effect for
     * non-existent elements.
     */
    template<class ForwardIterator>
    void increment_count(ForwardIterator begin, ForwardIterator end)
    {

        if (begin == end) {
            _count++;
            return;
        }

        item_t first = *begin;
        auto child = _map[first];
        if (child == nullptr)
            return;
        child->increment_count(begin + 1, end);
    }

    template<class Container>
    void increment_count(const Container &c)
    {
        increment_count(c.cbegin(), c.cend());
    }

    /**
     * Generate candidates at a given depth; assumes that frequent items of
     * size k-1 have already been computed.
    */
    void candidate_gen(std::size_t depth, std::size_t cur_depth=0)
    {
        if ((cur_depth + 2) == depth) {
            for (auto it1 = _map.begin(); it1 != _map.end(); ++it1) {
                auto it2 = it1;
                for (++it2; it2 != _map.end(); ++it2) {
                    auto min_max_pair = std::minmax(it1->first, it2->first);
                    auto lesser_child_node = _map[min_max_pair.first];
                    assert (lesser_child_node != nullptr);
                    lesser_child_node->create(min_max_pair.second);
                }
            }
        } else {
             for (auto &kv_pair : _map) {
                // TODO: early pruning of sub-trees of insufficient depth
                kv_pair.second->candidate_gen(depth, cur_depth + 1);
            }
        }
    }
    /**
     * Remove trie nodes with insufficient support.
     */
    void prune_candidates(std::size_t support)
    {
//        std::cout << "pre-prune: " << *this << std::endl;

        auto it = _map.begin();
        while (it != _map.end()) {
            // prune from the bottom up
            assert(it->second);
            it->second->prune_candidates(support);

            if (it->second->_count < support) {
                auto to_delete = it;
                ++it;
                _map.erase(to_delete);
            } else {
                ++it;
            }
        }
  //      std::cout << "post-prune: " << *this << std::endl;
    }


    void debug_print(std::ostream &out, int indent=0) const
    {
        std::cout << std::string(indent, ' ') << *this << std::endl;
        for (auto &kv_pair : _map) {
            kv_pair.second->debug_print(out, indent + 2);
        }
    }

    friend std::ostream &operator<<(std::ostream &out, const TrieNode &node);

private:

    TrieNode *create(item_t item)
    {
        auto &child = _map[item];
        assert (child == nullptr);
        child = new TrieNode{item};
        return child;
    }

    TrieNode *lookup_or_create(item_t item)
    {
        auto &child = _map[item];
        if (child == nullptr)            
            child = new TrieNode{item};
        return child;
    }

    item_t _item;
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
