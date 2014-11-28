#pragma once

#include <unordered_map>

class TrieNode {
    using item_t = int32_t;
    using MapType = std::unordered_map<item_t, TrieNode *>;

public:

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
     * Add an element to the Trie with zero count.
     */
    template<class ForwardIterator>
    void add_element(ForwardIterator begin, ForwardIterator end)
    {
        if (begin == end)
            return;
        item_t first = *begin;
        auto child = _map[first];
        if (child == nullptr) {
            // allocate
            _map[first] = child;
        }
        child->add_element(begin + 1, end);
    }

    template<class Container>
    void add_element(const Container &c)
    {
        add_element(c.cbegin(), c.cend());
    }

    /**
     * Increment the count of an existing Trie element; has not effect for
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

    friend std::ostream &operator<<(std::ostream &out, const TrieNode &node);

private:

    TrieNode *lookup_or_create(item_t item)
    {
        auto &child = _map[item];
        if (child == nullptr)            
            child = new TrieNode{};
        return child;
    }

    MapType _map{};
    std::size_t _count{};
};

std::ostream &operator<<(std::ostream &out, const TrieNode &node) {

    out << node._count << " [";
    for (auto kv_pair : node._map) {
        out << kv_pair.first << " ";
    }
    out << "]";

    return out;
}
