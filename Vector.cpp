//
// Created by DevilInChina on 2018/9/19.
//

#include <algorithm>
#include <cstdio>
#include <functional>
typedef unsigned int __Index;
template <class T>
class Vector{
public:
    typedef unsigned int __Index;
    static const __Index _beginSize = 16;
    class iterator{
    public:
        iterator operator++() {
            ++eleptr;
            return iterator(eleptr);
        }

        T*operator ->(){
            return *eleptr;
        }

        iterator operator--() {
            --eleptr;
            return iterator(eleptr);
        }

        T &operator*() {
            return **eleptr;
        }

        const iterator operator--(int) {
            iterator tmp(this->eleptr);
            operator--();
            return tmp;
        }

        const iterator operator++(int) {
            iterator tmp(this->eleptr);
            operator++();
            return tmp;
        }

        iterator() {
            eleptr = nullptr;
        }

        __Index operator-(const iterator &a)const{
            return eleptr - a.eleptr;
        }

        iterator operator+(int d)const{
            return iterator(eleptr+d);
        }

        iterator operator-(int d)const{
            return iterator(eleptr-d);
        }

        iterator(const iterator &X) {
            eleptr = X.eleptr;
        }

        iterator &operator=(const iterator &X) {
            eleptr = X.eleptr;
            return *this;
        }

        explicit iterator(T **X) {
            eleptr = X;
        }

        bool operator==(const iterator &X) const {
            return eleptr == X.eleptr;
        }

        bool operator!=(const iterator &X) const {
            return eleptr != X.eleptr;
        }
        friend Vector<T>;
    private:
        T **eleptr;
    };

    Vector(T*s,const T*e){
        Size = e-s;
        MaxSize = _beginSize;
        while (MaxSize < Size)MaxSize<<=1;
        data = new T*[MaxSize+1];
        for(__Index i = 0 ; i < Size ; ++i,++s){
            data[i] = new T(*s);
        }
    }

    Vector(){
        MaxSize = _beginSize;
        Size = 0;
        data = new T*[MaxSize+1];
    }

    Vector(Vector<T>&&temp)noexcept{
        Size = temp.Size;
        MaxSize = temp.MaxSize;
        data = temp.data;
    }

    Vector&operator=(Vector<T>&&temp)noexcept{
        Size = temp.Size;
        MaxSize = temp.MaxSize;
        data = temp.data;
    }

    void push_back(const T&ele){
        if(Size < MaxSize){
            data[Size++] = new T(ele);
        }else{
            MaxSize<<=1;
            T **tmp = new T*[MaxSize+1];
            std::copy(data,data+Size,tmp);
            delete []data;
            data = tmp;
            data[Size++] = new T(ele);
        }
    }

    void emplace_back(T&&ele){
        if(Size < MaxSize){
            data[Size++] = new T(std::move(ele));
        }else{
            MaxSize<<=1;
            T **tmp = new T*[MaxSize+1];
            std::copy(data,data+Size,tmp);
            delete []data;
            data = tmp;
            data[Size++] = new T(std::move(ele));
        }
    }

    __Index capacity(){
        return MaxSize;
    }

    void pop_back(){
        --Size;

        delete data[Size];
    }

    T&front(){
        return **data;
    }

    T&operator[](__Index i){
        return *data[i];
    }

    T&back(){
        return **(data+(Size?(Size-1):Size));
    }

    const T&operator[](__Index i)const{
        return *data[i];
    }

    bool empty()const{
        return !Size;
    }

    __Index size()const{
        return Size;
    }

    void clear(){
        for(__Index i = 0 ; i < Size ; ++i){
            delete data[i];
        }
        if(MaxSize!=_beginSize) {
            delete[]data;
            MaxSize = _beginSize;
            data = new T *[MaxSize + 1];
        }
        Size = 0;
    }

    iterator begin(){
        return iterator(data);
    }

    iterator end(){
        return iterator(data+Size);
    }

    ~Vector(){
        for(__Index i = 0 ; i < Size ; ++i){
            delete data[i];
        }
        delete []data;
    }

    void Swap(iterator &a, iterator &b){
        T*c = *a.eleptr;
        *a.eleptr = *b.eleptr;
        *b.eleptr = c;
    }

    void sort(std::function<bool(const T &a, const T &b)>
              cmp = [](const T&a,const T&b)->bool{
                  return a<b;
              }){
        quick_sort(begin(),end(),cmp);
    }

private:
    T **data;
    __Index MaxSize;
    __Index Size;
    void quick_sort(iterator s,iterator e,std::function<bool(const T& , const T &)> _Compare){
        iterator i = s;
        iterator j = e;
        if(i==j)return;
        --j;
        if(i==j)return;
        T key = *i;
        while (i!=j){
            while(!_Compare(*j,key)and i!=j)j--;
            while(!_Compare(key,*i)and i!=j)i++;
            if(i!=j){
                Swap(i, j);
            }
        }
        Swap(s, i);
        *i = key;
        quick_sort(s,i,_Compare);
        ++i;
        quick_sort(i,e,_Compare);
    }
};
