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
            this->wait();
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
                    this->wait();
                }
            }
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
        mergeSort(first,middle,comparator);
        mergeSort(middle,last,comparator);
        std::inplace_merge(first, middle, last,comparator);
        this->wait();
    }
    virtual ~MergeSort(){}
};

#endif // SORTS_H
