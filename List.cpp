//
// Created by DevilInChina on 2018/9/17.
//

#include <iostream>
#include <algorithm>
#include <functional>
#include <string>

typedef unsigned int __Index;

template <typename T>
class List {
private:
    struct Node{
        T element;
        Node *next;
        Node *pre;
        explicit Node(const T& X) : element(X) {
            next = pre = nullptr;
        }
        Node() {
            next = pre = nullptr;
        }
    };
    Node *ori;
public:

    struct iterator {///设置迭代器
        iterator operator++() {
            it = it->next;
            return iterator(it);
        }

        T*operator ->(){
            return &(it->element);
        }

        iterator operator--() {
            it = it->pre;
            return iterator(it);
        }

        T &operator*() {
            return it->element;
        }

        const iterator operator--(int) {
            iterator tmp(this->it);
            operator--();
            return tmp;
        }

        const iterator operator++(int) {
            iterator tmp(this->it);
            operator++();
            return tmp;
        }

        iterator() {
            it = nullptr;
        }

        iterator(const iterator &X) {
            it = X.it;
        }

        iterator &operator=(const iterator &X) {
            it = X.it;
            return *this;
        }

        explicit iterator(Node *X) {
            it = X;
        }

        bool operator==(const iterator &X) const {
            return it == X.it;
        }

        bool operator!=(const iterator &X) const {
            return it != X.it;
        }

        Node *it;
    };

    List() {
        ori = new Node();
        ori->next = ori;
        ori->pre = ori;
    }

    List(List<T>&&a)noexcept{
        ori = a.ori;
        a.ori = nullptr;
    }

    List(T *beg, const T *end) : List() {
        while (beg != end) {
            insert(iterator(ori->pre), *beg);
            ++beg;
        }
    }

    List(const List<T>&X):List(){
        Node *p;
        p = X.ori->next;
        while (p!=X.ori){
            push_back(p->element);
            p = p->next;
        }
    }

    ~List() {
        iterator it(begin().it->next);
        for (; it != end(); ++it) {
            if (it.it) {
                delete it.it->pre;
            }
        }
        delete ori;
    }

    unsigned int size(){
        unsigned int ans = 0;
        for(auto it = begin() ; it!=end() ; ++it){
            ++ans;
        }
        return ans;
    }

    iterator erase(iterator it) {
        if (it.it->next == it.it)return it;
        iterator next(it.it->next);
        next.it->pre = it.it->pre;
        it.it->pre->next = next.it;
        delete it.it;
        return next;
    }

    iterator erase(iterator &sta, iterator &tar) {
        iterator res(sta.it);
        while (res != tar) {
            res = erase(res);
        }
    }

    iterator begin() {
        return iterator(ori->next);
    }

    iterator end() {
        return iterator(ori);
    }

    bool empty() { return ori->next == ori; }

    void insert(iterator pos, const T &ele) {///在pos之后插入一个新的元素
        Node *temp = new Node(ele);
        temp->next = pos.it->next;
        pos.it->next->pre = temp;
        pos.it->next = temp;
        temp->pre = pos.it;
    }

    void reverse() {
        if (ori->next == ori)return;
        iterator temp(ori->pre), it(end()), temp2;
        while (it.it->next != temp.it) {
            temp2.it = temp.it->pre;
            insert(it, temp, temp);
            temp = temp2;
            ++it;
        }
    }

    void push_back(const T &element) {
        insert(iterator(ori->pre), element);
    }

    void push_front(const T &element) {
        insert(iterator(ori), element);
    }

    T&front(){
        return *(iterator(ori->next));
    }

    T&back(){
        return *(iterator(ori->pre));
    }

    bool pop_back(){
        if(!empty()) {
            erase(iterator(ori->pre));
            return true;
        }
        return false;
    }

    bool pop_front(){
        if(!empty()) {
            erase(iterator(ori->next));
            return true;
        }
        return false;
    }

    ///将链表的剪切入新链表,传入迭代器为本链表时应慎用，会造成内存垃圾
    void insert(iterator pos, iterator begPos, iterator endPos) {
        iterator pre(begPos.it->pre), next(endPos.it->next);
        endPos.it->next = pos.it->next;
        pos.it->next->pre = endPos.it;
        pos.it->next = begPos.it;
        begPos.it->pre = pos.it;
        pre.it->next = next.it;
        next.it->pre = pre.it;
    }

    iterator find(const T &a, std::function<bool(const T &a, const T &b)> cmp
    = [](const T &a, const T &b) -> bool { return a < b; }) {///自定义比较规则，成立的结果均为false判定为相等
        for (auto it = begin(); it != end(); ++it) {
            if (!cmp(a, *it) and !cmp(*it, a))return it;
        }
        return end();
    }

    void unique(std::function<bool(const T &a, const T &b)> cmp
    = [](const T &a, const T &b) -> bool { return a < b; }) {///自定义比较规则，成立结果有一为true判定为不相等
        auto tmp(new List<std::pair<T,int> >());
        std::function<bool(const T &a,const T &b)> equal
        = [&](const T &a, const T &b)->bool{
            return !cmp(a,b) and !cmp(b,a);
        };
        int order = 0;
        (*(tmp->end())).second = -1;///设置哨兵
        for(auto i = begin() ; i!=end() ; ++i){
            tmp->push_back(std::make_pair(*i,order));
            ++order;
        }///制作下标

        tmp->sort([&](const std::pair<T,int>&A,const std::pair<T,int>&B)-> bool{
            return equal(A.first,B.first)?(A.second<B.second):(cmp(A.first,B.first));
        });///二级排序

        auto it = tmp->begin();
        auto _next = tmp->begin();
        ++_next;

        for( ; _next!=tmp->end() ; ){///去重
            if(equal((*_next).first,(*it).first)){
                _next = tmp->erase(_next);
            }
            else {
                ++it;
                ++_next;
            }
        }
        tmp->sort([&](const std::pair<T,int>&A,const std::pair<T,int>&B)-> bool{
            return A.second <B.second;
        });///排序回原序
        order = 0;
        auto fit(tmp->begin());
        for(auto i = begin() ; i!=end(); ++order){///原链表去重
            if(order==(*fit).second){
                ++i;
                ++fit;
            } else {
                i = erase(i);
            }
        }
        delete tmp;
    }

    void sort(std::function<bool(const T &a, const T &b)> cmp
    = [](const T &a, const T &b) -> bool { return a < b; }){
        quick_sort(begin(),end(),cmp);
    }

    void splice(List &X) {///两链表拼接
        if (X.ori->next == X.ori)return;
        insert(iterator(ori->pre), iterator(X.ori->next), iterator(X.ori->pre));
    }

    void show(std::function<void(const T&a)> Fun
    =[](const T&a)->void{std::cout<<a<<" ";}) {///可以自定义输出形式
        unsigned int res = 0;
        for (auto it = begin(); it != end(); ++it) {
            ++res;
            Fun(*it);
        }
        std::cout << std::endl << "Size:" << res << std::endl;
    }

    void Shuffle(){///按特殊顺序打乱
        List<T>k;///辅助链表
        unsigned cnt = 0;
        for(auto i = begin() ; i!= end() ;++cnt) {
            if (cnt & 1) {
                auto tmp = i;
                ++i;
                k.insert(iterator(k.ori),tmp,tmp);///把原链表中的temp拆下来放到辅助链表
            }
            else ++i;
        }
        splice(k);///两链表拼接
    }

private:

    void quick_sort(iterator s,iterator e,std::function<bool(const T &a, const T &b)> cmp){
        iterator i = s;
        iterator j = e;
        if(i==j)return;
        --j;
        if(i==j)return;
        T key = *i;
        while (i!=j){
            while(!cmp(*j,key)and i!=j)j--;
            while(!cmp(key,*i)and i!=j)i++;
            if(i!=j){
                swap(*i,*j);
            }
        }
        *s = *i;
        *i = key;
        quick_sort(s,i,cmp);
        ++i;
        quick_sort(i,e,cmp);
    }
};
