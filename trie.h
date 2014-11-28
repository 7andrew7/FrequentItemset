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

private:

    TrieNode *lookup_or_create(item_t item)
    {
        auto child = _map[item];
        if (child == nullptr)            
            child = new TrieNode{};
        return child;
    }

    MapType _map{};
    std::size_t _count{};
};
