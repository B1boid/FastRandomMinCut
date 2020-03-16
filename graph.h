#pragma once

#include <cstdio>
#include <vector>
#include <map>
#include <fstream>


class graph {
public:

    graph();

    graph(const graph &copy);

    friend std::istream &operator>>(std::istream &is, graph &g);

    friend std::ostream &operator<<(std::ostream &os, const graph &g);

    // возвращает вес ребра
    double get(size_t u, size_t v) const;

    // присваиввает вес ребру
    void set(size_t u, size_t v, double w);

    // назначает все размеры
    void setup_graph(size_t size);

    // возварщает текущий размер графа
    size_t get_cur_size() const;

    // уменьшает размер графа
    void decrease_cur_size();

    // удаляет вершину из списка доступных вершин
    void delete_vertex(size_t v);

    //возращает вершину из списка доступных вершин
    size_t get_vertex(size_t v) const;

    // вовзращет величину минимального разреза
    double count_min_cut() const;

    graph &remove_self_loops();

    graph &merge_vertices(size_t u, size_t v);

    // проверяет добавлена ли при считывании эта вершина или нет
    bool is_already_added(size_t v) const;

    // добавляет вершину в словари для соответствия данных индексов вершин к програмным индесам
    void set_dict(size_t newV, size_t oldV);

    // возварщает по данному индексу вершины ее новый програмный индекс
    size_t get_new_vertex(size_t v);

    ~graph();


private:
    double *mData;
    size_t mSize;
    size_t mCurSize;
    std::vector<size_t> mVertices;
    std::map<size_t, size_t> toOldVertices;
    std::map<size_t, size_t> toNewVertices;
};