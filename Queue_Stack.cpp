//
// Created by DevilInChina on 2018/9/18.
//


#include "List.cpp"
template <typename T>
class ListQueue{
public:
    ListQueue():Data(){
        Size = 0;
    }

    void push(const T&a){
        ++Size;
        Data.push_back(a);
    }

    bool pop(){
        if(Size){
            --Size;
            Data.pop_front();
            return true;
        }
        return false;
    }

    bool empty(){
        return !Size;
    }

    T&front(){
        return Data.front();
    }

    unsigned int size(){ return Size;}

private:
    unsigned int Size;
    List<T>Data;
};

template <typename T>
class ListDoubleQueue{
public:
    ListDoubleQueue():Data(){Size = 0;}

    void show(std::function<void(const T&a)> Fun
    =[](const T&a)->void{std::cout<<a<<" ";}){
        Data.show(Fun);
    }

    void push_back(const T&a){
        ++this->Size;
        this->Data.push_back(a);
    }

    void push_front(const T&a){
        ++this->Size;
        this->Data.push_front(a);
    }

    bool pop_back(){
        if(this->Size){
            --this->Size;
            this->Data.pop_back();
            return true;
        }
        return false;
    }

    bool pop_front(){
        if(this->Size){
            --this->Size;
            this->Data.pop_front();
            return true;
        }
        return false;
    }

    T&back(){
        return this->Data.back();
    }

    bool empty(){
        return !Size;
    }

    T&front(){
        return this->Data.front();
    }

    unsigned int size(){
        return this->Size;
    }

private:
    unsigned int Size;
    List<T>Data;
};

#define BEGIN_SIZE 44971
template <typename T>
class Stack {
public:
    explicit Stack(unsigned int k = BEGIN_SIZE) : MaxSize(k ? k : 1) {
        data = new T[MaxSize];
        Top = 0;
    }

    void pop() {
        --Top;
    }

    void push(const T &ele) {
        data[Top++] = ele;
    }

    bool Pop_safe() {
        if (Top) {
            --Top;
            return true;
        }
        return false;
    }

    bool Push_safe(const T &ele) {
        if (Top + 1 < MaxSize) {
            data[Top++] = ele;
            return true;
        }
        return false;
    }

    bool empty() {
        return !Top;
    }

    unsigned int size() {
        return Top;
    }

    T &top() {
        return data[Top - 1];
    }

    void reverse() {
        auto tmp(new T[Top + 1]);
        unsigned int mak = 0;
        while (!empty()) {
            tmp[mak++] = top();
            pop();
        }
        for (unsigned int i = 0; i < mak; ++i) {
            push(tmp[i]);
        }
        delete[]tmp;
    }

    ~Stack() {
        delete[]data;
    }

private:
    __Index MaxSize;
    __Index Top;
    T *data;
};

template <typename T>
class Queue{
public:
    explicit Queue(unsigned int k = BEGIN_SIZE) : maxs(k ? k : 1) {
        data = new T[maxs];
        Back = Front = 0;
    }

    bool empty(){
        return Back==Front;
    }

    bool push(const T&element){
        __Index __next = (Back+1)%maxs;
        if(__next!=Front){
            data[Back] = element;
            Back = __next;
            return true;
        }
        else return false;
    }

    const T&front()const{
        return data[Front];
    }

    T&front(){
        return data[Front];
    }

    bool pop(){
        if(empty()){
            return false;
        }
        else {
            ++Front;
            Front%=maxs;
        }
    }

    __Index size(){
        if(Front <= Back){
            return Back-Front;
        } else {
            return maxs-Front+Back;
        }
    }

private:
    __Index maxs;
    __Index Back;
    __Index Front;
    T *data;
};

template <typename T>
class ListStack{
public:
    ListStack():Data(){
        Size = 0;
    }

    void push(const T&a){
        ++Size;
        Data.push_back(a);
    }

    bool pop(){
        if(Size){
            --Size;
            Data.pop_back();
            return true;
        }
        return false;
    }

    T&top(){
        return Data.back();
    }

    bool empty(){
        return Size==0;
    }

    unsigned int size(){ return Size;}

private:
    unsigned int Size;
    List<T>Data;
};
/**
std::string Trans(unsigned long long Number,int System){///10进制转N进制
    Stack<char>res(100);///
    std::string ret;
    if(System < 2 or System > 62)///超出表达范围
        return ret;
    std::function<char(unsigned long long )>fun = [](unsigned long long a)->char{
        if(a<10)return char(a+'0');
        else if(a<26)return char(a-10+'a');
        else return char(a-37+'A');
    };
    while (Number){
        res.push(fun(Number%System));
        Number/=System;
    }
    while (!res.empty()){
        ret.push_back(res.top());
        res.pop();
    }
    return ret;
}

*/