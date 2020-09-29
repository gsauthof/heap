#include <array>
#include <iostream>
#include <string.h>
#include <assert.h>

#include "../heap.hh"

struct Item {
    uint32_t i {4711}; // heap index
    std::array<char, 512> payload {0};
};

struct Table {
    enum { N = 174762 };
    Item *items[N] {0};
    // heap of used slots
    uint32_t heap[N] {0};
    uint32_t n {0};

    void insert(unsigned i, const char *s);
    Item *remove(unsigned i);
};
struct Swap_Item {
    static void swap(void *p, size_t i, size_t j, void *user)
    {
        Item **items = static_cast<Item**>(user);
        uint32_t *a = static_cast<uint32_t*>(p) + i;
        uint32_t *b = static_cast<uint32_t*>(p) + j;
        assert(items[*a]->i == i);
        assert(items[*b]->i == j);
        std::swap(*a, *b);
        items[*a]->i = i;
        items[*b]->i = j;
    }
};
struct Move_Item {
    static void move(void *p, size_t i, size_t j, void *user)
    {
        Item **items = static_cast<Item**>(user);
        uint32_t *a = static_cast<uint32_t*>(p) + i;
        uint32_t *b = static_cast<uint32_t*>(p) + j;
        *a = *b;
        items[*a]->i = i;
    }
};
void Table::insert(unsigned i, const char *s)
{
    Item *x = new Item;
    strcpy(x->payload.data(), s);
    items[i] = x;
    heap[n] = i;
    x->i = n;
    gms::heap::insert(heap, n++, gms::heap::Max<uint32_t>(), Swap_Item(), items);
}
Item *Table::remove(unsigned i)
{
    Item *x = items[i];
    items[i] = nullptr;
    assert(x);
    gms::heap::remove(heap, x->i, n--, gms::heap::Max<uint32_t>(), Move_Item(),
            Swap_Item(), items);
    return x;
}

int main(int argc, char **argv)
{
    unsigned n = atoi(argv[1]);
    Table t;
    for (unsigned i = 7; i < n; i += 7) {
        t.insert(i, "blah");
    }
    for (int i = 2; i < argc; ++i) {
        unsigned k = atoi(argv[i]);
        Item *x  = t.items[k];
        assert(x);
        std::cerr << "Removing k-th table entry: " << k
            << " (heap index: " << x->i << ")\n";
        assert(t.heap[x->i] == k);
        Item *a = t.remove(k);
        assert(a == x);
        delete a;
    }
    std::cout << '\n';
    while (t.n) {
        size_t k = t.heap[0];
        std::cerr << "Extracting max table entry: " << k << '\n';
        Item *a = t.items[k];
        assert(a->i == 0);
        Item *x = t.remove(k);
        assert(x == a);
        delete x;
    }
    return 0;
}


