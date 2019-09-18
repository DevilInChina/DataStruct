#include <iostream>
#include "Vector.cpp"
template <typename Element>
class Heap {
public:
    explicit Heap(std::function<bool(const Element&,const Element&)>temp
    = [](const Element&a,const Element&b)->bool{ return a<b;}):store(){
        Size = 0;
        _Compare = temp;
    }
    void push(const Element &a) {
        store.push_back(a);
        Size++;
        __Index i = Size - 1, j;
        while (i) {
            j = (i - 1) >> 1;
            if (!_Compare(store[j], a))break;
            store[i] = store[j];
            i = j;
        }
        store[i] = a;
    }

    const Element& top(){ return store[0]; }
    bool empty(){ return !Size;}
    void pop() {
        __Index i = 0,j=1;
        --Size;
        Element &mark = store[Size];
        while (j<=Size){
            if(j<Size and _Compare(store[j],store[j+1]))++j;
            if(!_Compare(mark,store[j]))break;
            store[i] = store[j];
            i = j,j=i<<1|1;
        }
        store[i] = mark;
        store.pop_back();
    }

    void Show(){
        auto it = store.begin();
        for(auto &i:store){
            i.Show();
        }
    }

    __Index size(){ return Size;}

private:
    Vector<Element>store;
    __Index Size;
    std::function<bool(const Element&,const Element&)>_Compare;
};
