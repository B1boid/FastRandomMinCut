#include <iostream>
#include <fstream>
#include <cmath>
#include "graph.h"

// максимальное время выполнения программы в секундах
double const MAX_TIME = 1200;


double minimum_cut = 1000000000, start_time;

// проверяет не исчерпан ли лимит по времени, если исчерпан возварщает текущий посчитанный ответ
void checkTime() {
    if ((double) (clock() - start_time) / CLOCKS_PER_SEC > MAX_TIME) {
        std::ofstream ofs;
        ofs.open("output.csv");
        ofs << (minimum_cut / 2);
        ofs.close();
        exit(0);
    }
}


//сжимаем граф до заданного размера t
void random_contraction(graph &g, size_t t) {
    g.remove_self_loops();

    while (g.get_cur_size() > t) {
        size_t random_u = 0, random_v = 0;
        do {
            random_u = g.get_vertex(rand() % g.get_cur_size());
            random_v = g.get_vertex(rand() % g.get_cur_size());
        } while (g.get(random_u, random_v) == 0);

        g.merge_vertices(random_u, random_v);
        g.remove_self_loops();
    }

}

// эта функция будет вызываться в рекурсии, когда в графе будет меньше 7 вершин
double slowMinCut(graph &g) {
    size_t n = g.get_cur_size();
    float ln = log((float) n);
    size_t runs = n * n * ln;

    for (size_t i = 0; i < runs; ++i) {
        graph copy = g;
        random_contraction(copy, 2);
        double cut = copy.count_min_cut();
        if (cut < minimum_cut) {
            minimum_cut = cut;
        }
    }

    return minimum_cut;
}


//рекурсивная оптимизация
double fastMinCut(graph &g) {
    checkTime();
    //чтобы получить более точный минимальный разрез, будет вызывать slowMinCut когда остается 6 и менее вершин в графе
    if (g.get_cur_size() <= 6) {
        return slowMinCut(g);
    }

    // будем уменьшать граф в корень из двух раз
    size_t t = g.get_cur_size() / sqrt(2);

    //запускаем независимо для двух одинаковых графов randomContraction
    graph copy1 = g;
    graph copy2 = g;
    g.~graph();

    random_contraction(copy1, t);
    random_contraction(copy2, t);

    // берем минимальный разрез из двух получившихся при рекурсии
    return std::min(fastMinCut(copy1), fastMinCut(copy2));

}

int main(int argc, char *argv[]) {

    if (argc < 3) {
        std::cout << "Program arguments must have input and output file names" << std::endl;
        return 0;
    }
    start_time = clock();
    graph g;
    std::ifstream ifs(argv[1]);
    ifs >> g;
    ifs.close();

    srand((unsigned int) time(nullptr));

    std::cout << "Number of vertices:" << g.get_cur_size() << std::endl;
    double reading_time = (double) (clock() - start_time) / CLOCKS_PER_SEC;
    std::cout << "Reading is over after:" << reading_time << "sec" << std::endl;
    minimum_cut = fastMinCut(g);

    std::cout << "Algo is over after:" << (double) (clock() - reading_time) / CLOCKS_PER_SEC << std::endl;
    std::cout << "Min cut:" << minimum_cut / 2 << std::endl;
    std::ofstream ofs;
    ofs.open(argv[2]);
    ofs << (minimum_cut / 2);
    ofs.close();
}