#ifndef SORTS_H
#define SORTS_H
#include <functional>
#include <string>
#include <algorithm>
#include <stdlib.h>
#include <thread>
#include <chrono>

struct Stopper{
    bool stopped = true;
    void wait(){
        while(stopped){
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
        stopped = true;
    }
};

template<class ITERATOR, class COMPARATOR = std::less<typename std::iterator_traits<ITERATOR>::value_type>>
struct SortingAlgorithm{
    virtual void sort(const ITERATOR first, const ITERATOR last, const COMPARATOR comparator = COMPARATOR ())=0;
    virtual void setStopper(Stopper *stopper){
        this->stopper = stopper;
    }
    virtual void wait(){
        if(stopper != nullptr){
            stopper->wait();
        }
    }
    virtual ~SortingAlgorithm(){}
    Stopper *stopper = nullptr;
};

template<class ITERATOR, class COMPARATOR = std::less<typename std::iterator_traits<ITERATOR>::value_type>>
struct InsertionSort: public SortingAlgorithm<ITERATOR, COMPARATOR>{
    virtual void sort(const ITERATOR first, const ITERATOR last, const COMPARATOR comparator = COMPARATOR ()) override{
        if(first == last || std::next(first) == last) return;
        for(auto iterator = std::next(first);iterator!=last;++iterator){
            const auto insertionIterator = std::upper_bound(first,iterator,*iterator,comparator);
            std::rotate(insertionIterator,iterator,std::next(iterator));
            SortingAlgorithm<ITERATOR, COMPARATOR>::wait();
        }
    }
    virtual ~InsertionSort(){}
};

template<class ITERATOR, class COMPARATOR = std::less<typename std::iterator_traits<ITERATOR>::value_type>>
struct SelectionSort: public SortingAlgorithm<ITERATOR, COMPARATOR>{
    virtual void sort(const ITERATOR first, const ITERATOR last, const COMPARATOR comparator = COMPARATOR ()) override{
        if(first == last || std::next(first) == last) return;
        for(auto iterator = first;iterator!=last;++iterator){
            const auto minValueIterator = std::min_element(iterator,last,comparator);
            std::swap(*iterator,*minValueIterator);
            SortingAlgorithm<ITERATOR, COMPARATOR>::wait();
        }
    }
    virtual ~SelectionSort(){}
};

template<class ITERATOR, class COMPARATOR = std::less<typename std::iterator_traits<ITERATOR>::value_type>>
struct BubbleSort: public SortingAlgorithm<ITERATOR, COMPARATOR>{
    virtual void sort(const ITERATOR first, const ITERATOR last, const COMPARATOR comparator = COMPARATOR ()) override{
        if(first == last || std::next(first) == last) return;
        auto end = std::prev(last);
        bool isSorted = true;
        do{
            isSorted = true;
            for(auto iterator = first;iterator!=end;++iterator){
                if(!comparator(*iterator,*std::next(iterator))){
                    std::swap(*iterator,*std::next(iterator));
                    isSorted = false;
                }
            }            
            SortingAlgorithm<ITERATOR, COMPARATOR>::wait();
        }while(--end,!isSorted);
    }
    virtual ~BubbleSort(){}
};

template<class ITERATOR, class COMPARATOR = std::less<typename std::iterator_traits<ITERATOR>::value_type>>
struct MergeSort: public SortingAlgorithm<ITERATOR, COMPARATOR>{
    virtual void sort(const ITERATOR first, const ITERATOR last, const COMPARATOR comparator = COMPARATOR ()) override{
        if(first == last || std::next(first) == last) return;
        const auto length = std::distance(first,last);
        const auto middle = std::next(first,length/2);
        sort(first,middle,comparator);
        sort(middle,last,comparator);
        std::inplace_merge(first, middle, last,comparator);
        SortingAlgorithm<ITERATOR, COMPARATOR>::wait();
    }
    virtual ~MergeSort(){}
};

template<class ITERATOR, class COMPARATOR = std::less<typename std::iterator_traits<ITERATOR>::value_type>>
struct QuickSort: public SortingAlgorithm<ITERATOR, COMPARATOR>{
    virtual void sort(const ITERATOR first, const ITERATOR last, const COMPARATOR comparator = COMPARATOR ()) override{        if(first == last || std::next(first) == last) return;
        const auto length = std::distance(first,last);
        const auto pivot = *std::next(first,rand()%length);;
        auto partitions = threeWayPartitioning(first,last,pivot,comparator);
        sort(first,partitions.at(0),comparator);
        sort(partitions.at(1),last,comparator);
    }
    std::array<ITERATOR,2> threeWayPartitioning(const ITERATOR first,const ITERATOR last,const typename std::iterator_traits<ITERATOR>::value_type value, const COMPARATOR comparator = COMPARATOR ()){
        auto start = first;
        auto end = std::prev(last);
        for(auto iterator = first;iterator!=std::next(end);){
            if(comparator(*iterator,value)){
                std::swap(*iterator++,*start++);
                SortingAlgorithm<ITERATOR, COMPARATOR>::wait();
            }
            else if(comparator(value,*iterator)){
                std::swap(*iterator,*end--);
                SortingAlgorithm<ITERATOR, COMPARATOR>::wait();
            }
            else ++iterator;
        }
        return std::array<ITERATOR,2>{start,end};
    }
};
#endif // SORTS_H
