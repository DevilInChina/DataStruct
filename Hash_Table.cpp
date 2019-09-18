//
// Created by DevilInChina on 2018/10/10.
//
#define FIND_CIRCLE 14
#define LENGTH_TYPE 13
#include "MiniPath.cpp"
template <class Index,class Data>
class Hash_Table {
protected:
typedef typename List<std::pair<Index,Data> >::iterator iterator;
    bool Equal(const Index&a,const Index&b){
        return !(a<b) and !(b<a);
    }
    const __Index Length[LENGTH_TYPE] = {263, 523, 1031, 2063, 4099, 8219, 16411, 32771, 65539, 131111, 262147, 524347, 1048583};
    ///所有素数%4==3 在这种情况下进行平方探测会有较好的结果
    const __Index Gap[FIND_CIRCLE] = {0, 1, 4, 9, 16, 25, 36, 49, 64, 81, 100, 121, 144, 169};
    ///搜错循环长度，超出此长度认为常数过大，则应构建新的HashTable
    std::pair<const Index&, iterator > **Code;///first for number,second for markdown
    __Index curMaxSize, curRate;

    /**
     * @note 重新对内存进行分配
     */
    void memAlloc() {
        if(Code)for(int i = 0; i < curMaxSize ;++i)delete Code[i];
        curMaxSize = Length[curRate];
        delete[]Code;
        Code = new std::pair<const Index&, iterator >*[curMaxSize];
        std::fill(Code,Code+curMaxSize, nullptr);
    }

    /**
     * @param HashCode 对应的Hash编码
     * @param DataLoc 对应Hash码得到的数据位置(DataLoc>=0)
     * @return 是否插入成功
     */
    int Insert(const Index&Ins, iterator it ,__Index HashCode) {
        __Index __copy = HashCode;
        for (__Index i:Gap) {
            HashCode += i;
            HashCode %= curMaxSize;
            if (!Code[HashCode]) {

                Code[HashCode] = new std::pair<const Index&, iterator>(it->first,it);
                return 1;
            }else {
                if (Equal(Code[HashCode]->first, Ins))return -1;
            }
        }
        return 0;
    }

    /**
     * @param Ins 索引值
     * @param HashCode 索引值的哈希码
     * @return 相应list的迭代器
     */
    iterator Erase(const Index &Ins,__Index HashCode){
        __Index c = HashCode;
        for (__Index i:Gap) {
            c+=i;
            c%=curMaxSize;
            if(Code[c] and Equal(Code[c]->first,Ins)){
                auto it = Code[c]->second;
                delete Code[c];
                Code[c] = nullptr;
                return it;
            }
        }
        return iterator(nullptr);
    }

    iterator Find(const Index&Ins,__Index HashCode){
        __Index c = HashCode;
        for (__Index i:Gap) {
            c+=i;
            c%=curMaxSize;
            if(Code[c] and Equal(Code[c]->first,Ins)){
                return Code[c]->second;
            }
        }
        return iterator(nullptr);
    }

public:
    explicit Hash_Table(__Index StartSize = 0) {
        if (StartSize >= LENGTH_TYPE)StartSize = LENGTH_TYPE-1;
        curRate = StartSize;
        Code = nullptr;
        curMaxSize = Length[StartSize];
        memAlloc();

    }
    ~Hash_Table(){
        for(int i = 0 ; i < curMaxSize ; ++i)delete Code[i];
        delete []Code;
        Code = nullptr;
    }
};


/**
 * @tparam Index
 * @tparam Data
 * @node 使用时需要定义 unsigned int operator%(const Index&,unsigned int)
 */
template <class Index,class Data>
class Unordered_map:public Hash_Table<Index,Data>{
private:
typedef std::function<__Index (const Index& a,__Index)> HashFun;
typedef typename List<std::pair<Index,Data> >::iterator iterator;
    List<std::pair<Index,Data>>store;
   HashFun hashFun;
public:
    explicit Unordered_map(HashFun func = [](const Index&fi,__Index mod)->__Index{
        return fi%mod;
    }):Hash_Table<Index,Data>(),hashFun(func){};
    void insert(const std::pair<Index,Data>mk){
        store.push_front(mk);
        int flag = this->Insert(mk.first,store.begin(),hashFun(mk.first,this->curMaxSize));
        switch (flag) {
            case 1:///正常插入
                break;
            case -1:///之前已插入
                store.pop_front();
                break;
            case 0: {///插入异常 需要扩容
                bool can;
                do {
                    can = false;
                    ++this->curRate;
                    this->memAlloc();
                    for (auto it = store.begin(); it != store.end(); ++it) {
                        if (this->Insert(it->first, it, hashFun(it->first,this->curMaxSize)))continue;
                        else {
                            can = true;
                            break;
                        }
                    }
                } while (can);
            }
                break;
            default:
                break;
        }

    }

    void erase(const Index&fi){
        auto it = this->Erase(fi,hashFun(fi,this->curMaxSize));
        if(it!= iterator(nullptr)){
            store.erase(it);
        }
    }

    Data&operator[](const Index&fi){
        __Index code = hashFun(fi,this->curMaxSize);
        auto it = this->Find(fi,code);
        if(it!=iterator(nullptr)){
            return it->second;
        }else{
            insert(std::make_pair(fi,Data()));
            return this->Find(fi,code)->second;
        }
    }

    iterator find(const Index&fi){
        auto it = this->Find(fi,hashFun(fi,this->curMaxSize));
        if(it!=iterator(nullptr))return it;
        else return store.end();
    }

    iterator begin(){
        return store.begin();
    }

    iterator end(){
        return store.end();
    }

    void Show(){
        for(auto c:store){
            std::cerr<<c.first<<" "<<c.second<<"\n";
        }
    }
};