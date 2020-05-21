#ifndef Included_NameModel_Shuffle_Set

#define Included_NameModel_Shuffle_Set


#include <bits/stdc++.h>
using namespace std;



template <typename T>
class shuffle_set{
private:
    vector<T> vec_;
    unordered_set<T> set_;
public:
    shuffle_set();
    template <typename Iterator>
    shuffle_set(Iterator begin, Iterator end);
    ~shuffle_set();
    void clear();
    bool empty();
    template <typename Iterator>
    void init(Iterator begin, Iterator end);
    bool insert(T newelem);
    T take_random_elem();
};

// #include<bits/stdc++.h>
// #include "shuffle_set.h"

// using namespace std;

template <typename T>
shuffle_set<T>::shuffle_set(){}


template <typename T>
template <typename Iterator>
shuffle_set<T>::shuffle_set(Iterator begin, Iterator end){
    init(begin, end);
}

template <typename T>
shuffle_set<T>::~shuffle_set(){
    vec_.clear();
    set_.clear();
}

template <typename T>
void shuffle_set<T>::clear(){
    vec_.clear();
    set_.clear();
}

template <typename T>
bool shuffle_set<T>::empty(){
    return vec_.empty();
}

template <typename T>
template <typename Iterator>
void shuffle_set<T>::init(Iterator begin, Iterator end){
    vec_.clear();
    set_.clear();
    for (auto it = begin; it != end; it++){
        if (set_.find(*it) != set_.end()) continue;
        vec_.push_back(*it);
        set_.insert(*it);
    }
}

template <typename T>
bool shuffle_set<T>::insert(T newelem){
    if (set_.find(newelem) != set_.end()) return false;
    set_.insert(newelem);
    vec_.push_back(newelem);
    return true;
}

template <typename T>
T shuffle_set<T>::take_random_elem(){
    if (set_.empty()) return *set_.begin();
    int rand_place = rand() % vec_.size();
    T ret = vec_[rand_place];
    vec_[rand_place] = vec_[vec_.size()-1];
    vec_.pop_back();
    set_.erase(ret);
    return ret;
}
#endif