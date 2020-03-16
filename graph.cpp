#include "graph.h"
#include <iostream>
#include <fstream>


graph::graph() {}

graph::graph(const graph &copy) {

    mVertices = copy.mVertices;
    mData = new double[copy.mSize * copy.mSize];
    mSize = copy.mSize;
    mCurSize = copy.mCurSize;
    memcpy(mData, copy.mData, sizeof(double) * mSize * mSize);
}

graph::~graph(){
    delete mData;
}

// вспомогательная функция записи в словарь
void convert_to_real_graph(graph &g, size_t vertex, size_t &realVertex, size_t &curV) {
    if (g.is_already_added(vertex)) {
        realVertex = g.get_new_vertex(vertex);
    } else {
        realVertex = curV;
        g.set_dict(curV, vertex);
        curV++;
    }
}

//чтение графа
std::istream &operator>>(std::istream &is, graph &g) {

    std::vector<std::pair<size_t, std::pair<size_t, double>>> temp;
    size_t curV = 0;
    while (!is.eof()) {
        std::string s;
        is >> s;
        if (!s.empty() && isdigit(s[0])) {
            size_t u = 0, v = 0, realU = 0, realV = 0;
            double w;
            std::string substr;
            bool isFirst = true;
            for (auto ch:s) {
                if (ch != ',') {
                    substr += ch;
                } else if (isFirst) {
                    u = stoi(substr);
                    isFirst = false;
                    substr = "";
                } else {
                    v = stoi(substr);
                    substr = "";
                }
            }
            w = stod(substr);
            convert_to_real_graph(g, u, realU, curV);
            convert_to_real_graph(g, v, realV, curV);
            temp.push_back({realU, {realV, w}});
            std::cout << realU << " " << realV << " " << w << std::endl;
        }
    }

    g.setup_graph(curV);
    for (auto data:temp) {
        size_t u, v;
        double w;
        u = data.first;
        v = data.second.first;
        w = data.second.second;
        if (u != v) {
            g.set(u, v, w);
            g.set(v, u, w);
        }
    }

    return is;
}

std::ostream &operator<<(std::ostream &os, const graph &g) {
    size_t sz = g.get_cur_size();

    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = 0; j < sz; ++j) {
            os << g.get(i, j) << " ";
        }

        os << std::endl;
    }
    return os;
}


double graph::get(size_t u, size_t v) const { return mData[(u * mSize + v)]; }

void graph::set(size_t u, size_t v, double w) { mData[(u * mSize + v)] = w; }

void graph::setup_graph(size_t size) {
    mSize = size;
    mCurSize = size;
    mData = new double[size * size];
    for (int i = 0; i < mSize; ++i) {
        mVertices.push_back(i);
    }
}

size_t graph::get_cur_size() const { return mCurSize; }

void graph::decrease_cur_size() { if (mCurSize > 0) mCurSize--; }

void graph::delete_vertex(size_t v) { mVertices.erase(remove(mVertices.begin(), mVertices.end(), v), mVertices.end()); }

size_t graph::get_vertex(size_t v) const { return mVertices[v]; }

bool graph::is_already_added(size_t v) const { return toNewVertices.count(v) > 0; }

void graph::set_dict(size_t newV, size_t oldV) {
    toOldVertices.insert({newV, oldV});
    toNewVertices.insert({oldV, newV});
}

size_t graph::get_new_vertex(size_t v) { return toNewVertices[v]; }

// считаем величину минимального разреза
double graph::count_min_cut() const {
    double n = 0;

    for (size_t i = 0; i < mCurSize; ++i) {
        for (size_t j = 0; j < mCurSize; ++j) {
            n = n + get(get_vertex(i), get_vertex(j));
        }
    }

    return n;
}

// убриаем петли
graph &graph::remove_self_loops() {
    for (size_t i = 0; i < mSize; ++i) {
        set(i, i, 0);
    }
    return *this;
}

// превращаем две вершины в одну, перевешивая все ребра
graph &graph::merge_vertices(size_t u, size_t v) {
    for (size_t i = 0; i < mSize; ++i) {
        double e = get(v, i);
        set(v, i, 0);
        set(i, v, 0);

        double n = e + get(u, i);
        set(u, i, n);
        set(i, u, n);
    }

    delete_vertex(v);
    decrease_cur_size();
    return *this;
}