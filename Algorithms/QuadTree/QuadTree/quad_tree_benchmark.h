#ifndef QUAD_TREE_BENCHMARK_H
#define QUAD_TREE_BENCHMARK_H
#include <ctime>
#include <chrono>
#include <set>
#include <cmath>

#include "quad_tree.h"
#include "quad_tree_slow.h"
#include "quad_tree_moderate.h"

enum QUAD_TREE_BENCHMARK_TYPE{SET_ELEMENTS,GET_OVERLAPPING_ELEMENTS,GET_ALL_OVERLAPPING_TUPLES};

template <class T>
class QuadTreeDataGenerator{
public:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;

public:
    QuadTreeDataGenerator(){
        srand(time(0));
    }
    ELEMENTS_PTR makeElements(int numberOfElements,
                              const AABB<T> &boundingBox = AABB<T>(0,0,799,799),
                              T minSize = 10,
                              T maxSize = 50)
    {
        ELEMENTS_PTR elements(numberOfElements);
        for(int i=0;i<numberOfElements;i++){
            T boundingBoxWidth = boundingBox.xMax - boundingBox.xMin;
            T boundingBoxHeight = boundingBox.yMax - boundingBox.yMin;
            T x = boundingBox.xMin + std::fmod(rand(),boundingBoxWidth);
            T y = boundingBox.yMin + std::fmod(rand(),boundingBoxHeight);
            T width = minSize + std::fmod(rand(),(maxSize-minSize));
            T height = minSize + std::fmod(rand(),(maxSize-minSize));
            elements.at(i) = QuadTreeElement<T>::makeElement(AABB<T>(x,y,x+width,y+height));
        }
        return  elements;
    }
};

template <class T>
class QuadTreeBenchmark{
public:
    typedef typename QuadTree<T>::ELEMENTS_PTR ELEMENTS_PTR;
    typedef typename QuadTree<T>::ELEMENT_PTR ELEMENT_PTR;

public:
    void testQuadTree(QuadTree<T>* quadTree,
                      int numberOfTests,
                      int numberOfElements,
                      std::set<QUAD_TREE_BENCHMARK_TYPE> benchmarkTypes = {
                            QUAD_TREE_BENCHMARK_TYPE::SET_ELEMENTS,
                            QUAD_TREE_BENCHMARK_TYPE::GET_OVERLAPPING_ELEMENTS,
                            QUAD_TREE_BENCHMARK_TYPE::GET_ALL_OVERLAPPING_TUPLES},
                      int treeDepth = 6,
                      int maxElementsPerBox = 6,
                      const AABB<T> &boundingBox = AABB<T>(0,0,799,799),
                      T minSize = 10,
                      T maxSize = 50 ) const
    {
        std::string typeName = typeid(*quadTree).name();
        std::cout<<"Performing "<<numberOfTests<<" test on '"<<typeName<<"' with "<<numberOfElements<<" elements..."<<std::endl;

        ELEMENTS_PTR elements = QuadTreeDataGenerator<T>().makeElements(
                    numberOfElements,
                    boundingBox,
                    minSize,
                    maxSize);
        quadTree->setElements(elements,boundingBox, treeDepth, maxElementsPerBox);

        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        if(benchmarkTypes.count(QUAD_TREE_BENCHMARK_TYPE::SET_ELEMENTS)>0){
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            for(int i=0;i<numberOfTests;i++){
                quadTree->setElements(elements,boundingBox, treeDepth, maxElementsPerBox);
            }
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
            std::cout<<"setElements took "<<duration<<" miliseconds. Average: "<<(double) duration/(double) numberOfTests<<" ms per single test"<<std::endl;
        }
        if(benchmarkTypes.count(QUAD_TREE_BENCHMARK_TYPE::GET_OVERLAPPING_ELEMENTS)>0){
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            for(int i=0;i<numberOfTests;i++){
                volatile int overlappingElementsNum = quadTree->getAllOverlappingElements().size();
            }
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
            std::cout<<"getElementsThatOverlap took "<<duration<<" miliseconds. Average: "<<(double) duration/(double) numberOfTests<<" ms per single test"<<std::endl;
        }
        if(benchmarkTypes.count(QUAD_TREE_BENCHMARK_TYPE::GET_ALL_OVERLAPPING_TUPLES)>0){
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            for(int i=0;i<numberOfTests;i++){
                volatile int overlappingElementsNum = quadTree->getAllOverlappingElementTuples().size();
            }
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
            std::cout<<"getAllOverlappingElementTuples "<<duration<<" miliseconds. Average: "<<(double) duration/(double) numberOfTests<<" ms per single test"<<std::endl;
        }
    }

    void printElementConstructionStats() const{
        std::cout<<"   Elements(): "<<QuadTreeElement<T>::countDefaultConstructor<<
                   ", Elements(args): "<<QuadTreeElement<T>::countConstructor<<
                   ", Elements(const &args): "<<QuadTreeElement<T>::countCopyConstructor<<
                   ", Elements(&&args): "<<QuadTreeElement<T>::countMoveConstructor<<std::endl;
    }
};

#endif // QUAD_TREE_BENCHMARK_H
