#include <QApplication>
#include <QWidget>

#include "main_window.h"
#include "quad_tree_benchmark.h"


using namespace std;

typedef int NUM;

int main(int argc, char *argv[])
{
    bool benchmark = true;
    if(benchmark){
        QuadTree<NUM> *quadTree;
//        quadTree = new QuadTreeModerate<NUM>();
//                QuadTreeBenchmark<NUM>().testQuadTree(
//                            quadTree,
//                            200,
//                            9900,
//                            {
//                                QUAD_TREE_BENCHMARK_TYPE::SET_ELEMENTS,
//                                QUAD_TREE_BENCHMARK_TYPE::GET_ALL_OVERLAPPING_TUPLES,
//                                QUAD_TREE_BENCHMARK_TYPE::GET_OVERLAPPING_ELEMENTS},
//                            10,
//                            10,
//                            AABB<NUM>(0,0,9999,9999)
//                        );
//        delete quadTree;
        quadTree = new QuadTreeFast<NUM>();
                QuadTreeBenchmark<NUM>().testQuadTree(
                            quadTree,
                            50,
                            9900,
                            {
                                QUAD_TREE_BENCHMARK_TYPE::SET_ELEMENTS,
                                QUAD_TREE_BENCHMARK_TYPE::GET_ALL_OVERLAPPING_TUPLES,
                                QUAD_TREE_BENCHMARK_TYPE::GET_OVERLAPPING_ELEMENTS},
                            10,
                            10,
                            AABB<NUM>(0,0,1999,1999)
                        );
        delete quadTree;
    }else{
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
