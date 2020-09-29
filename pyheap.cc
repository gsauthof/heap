// SPDX-FileCopyrightText: © 2020 Georg Sauthoff <mail@gms.tf>
// SPDX-License-Identifier: BSL-1.0

#include <pybind11/pybind11.h>
// copies/converts std::vector etc.
#include <pybind11/stl.h>


#include "heap.hh"


static std::vector<int> build_max_intP(std::vector<int> &v)
{
    gms::heap::build(v.data(), v.size());
    return v;
}
static bool is_heap_max_intP(std::vector<int> &v)
{
    return gms::heap::is_heap(v.data(), 0, v.size());
}

static std::vector<int> build_min_intP(std::vector<int> &v)
{
    gms::heap::build(v.data(), v.size(), gms::heap::Min<int>(), gms::heap::Swap<int>());
    return v;
}
static bool is_heap_min_intP(std::vector<int> &v)
{
    return gms::heap::is_heap(v.data(), 0, v.size(), gms::heap::Min<int>());
}
static std::vector<int> insert_max_intP(std::vector<int> &v, int x)
{
    v.push_back(x);
    gms::heap::insert(v.data(), v.size()-1);
    return v;
}
static std::vector<int> remove_max_intP(std::vector<int> &v, size_t i)
{
    gms::heap::remove(v.data(), i, v.size());
    v.resize(v.size()-1);
    return v;
}

PYBIND11_MODULE(heap, m) {
    m.doc() = "test heap functions";
    m.def("build_max_int", &build_max_intP, "build max-heap of ints");
    m.def("is_heap_max_int", &is_heap_max_intP, "check max-heap property");
    m.def("build_min_int", &build_min_intP, "build min-heap of ints");
    m.def("is_heap_min_int", &is_heap_min_intP, "check min-heap property");
    m.def("insert_max_int", &insert_max_intP, "insert into max-heap");
    m.def("remove_max_int", &remove_max_intP, "remove from max-heap");
}
