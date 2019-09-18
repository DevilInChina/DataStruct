//
// Created by DevilInChina on 2018/10/8.
//


#include <fstream>
#include <unordered_map>
#include "Graphic.cpp"
template<class Element>
class Tree {
private:
    std::function<bool (const Element&,const Element&)> _Compare;
    struct Node;
public:
    explicit Tree(std::function<bool (const Element&,const Element&)>&&
    Copy = [](const Element&a,const Element&b)->bool{ return a<b;}):_Compare(Copy){
        root = nullptr;
        Size = 0;
    }

    ~Tree(){
        ListQueue<Node*>Q;
        if(root)Q.push(root);
        while (!Q.empty()){
            auto it = Q.front();
            Q.pop();
            if(it->left)Q.push(it->left);
            if(it->right)Q.push(it->right);
            delete it;
        }
    }

    class iterator {///和红黑树中的迭代器几乎相同
    public:
        iterator() {
            PtrNode = nullptr;
        }

        explicit iterator(Node *Copy) {
            PtrNode = Copy;
        }

        iterator(const iterator &Copy) {
            PtrNode = Copy.PtrNode;
        }

        bool operator==(const iterator &Temp) {
            return PtrNode == Temp.PtrNode;
        }

        bool operator!=(const iterator &Temp) {
            return PtrNode != Temp.PtrNode;
        }

        const Element &operator*() const {
            return PtrNode->element;
        }

        Element &operator*() {
            return PtrNode->element;
        }

        iterator operator++() {
            if (PtrNode->right) {
                PtrNode = PtrNode->right;
                while (PtrNode->left) {
                    PtrNode = PtrNode->left;
                }
                return *this;
            } else {
                while (PtrNode->father and PtrNode != PtrNode->father->left) {
                    PtrNode = PtrNode->father;
                }
                PtrNode = PtrNode->father;
                return *this;
            }
        }

        iterator operator--() {
            if (PtrNode->left) {
                PtrNode = PtrNode->left;
                while (PtrNode->right) {
                    PtrNode = PtrNode->right;
                }
                return *this;
            } else {
                while (PtrNode->father and PtrNode != PtrNode->father->right) {
                    PtrNode = PtrNode->father;
                }
                PtrNode = PtrNode->father;
                return *this;
            }
        }

    private:
        friend Tree<Element>;
        Node *PtrNode;
    };

    iterator end(){ return iterator(nullptr);}

    iterator begin(){
        return iterator(TreeMin(root));
    }

    iterator find(const Element&temp){
        Node *ptr = root;
        while (ptr!= nullptr){
            if(_Compare(ptr->element,temp)){
                ptr = ptr->right;
            }
            else if(_Compare(temp,ptr->element)){
                ptr = ptr->left;
            }
            else return iterator(ptr);
        }
        return iterator(nullptr);
    }

    void LevelTraverse(std::function<void(const Element&)>Show=
    [](const Element&temp)->void{
        std::cout<<temp<<" ";
    }) {
        ListQueue<Node*>Q;
        if(root)Q.push(root);
        while (!Q.empty()){
            auto it = Q.front();
            if(it->left)Q.push(it->left);
            if(it->right)Q.push(it->right);
            Show(it->element);
            Q.pop();
        }
    }
    void PreOrderTraverse(std::function<void(const Element&)>Show=
    [](const Element&temp)->void{
        std::cout<<temp<<" ";
    }){
        ListStack<Node*>St;
        if(root== nullptr)return;
        St.push(root);
        while (!St.empty()){
            auto it = St.top();
            St.pop();
            if(it->right)St.push(it->right);
            if(it->left)St.push(it->left);
            Show(it->element);
        }
    }

    void InOrderTraverse(std::function<void(const Element&)>Show=
    [](const Element&temp)->void{
        std::cout<<temp<<" ";
    }){
        ListStack<Node*>St;
        auto _next = root;
        while (_next || !St.empty()){
            if(_next){
                St.push(_next);
                _next = _next->left;
            }else{
                _next = St.top();
                Show(_next->element);
                St.pop();
                _next = _next->right;
            }
        }
    }

    void PostOrderTraverse(std::function<void(const Element&)>Show=
    [](const Element&temp)->void{
        std::cout<<temp<<" ";
    }){
        if(!root)return;
        Node *p = root ,*last = root;
        ListStack<Node*>St;
        St.push(p);
        while (!St.empty()){
            p = St.top();
            if((!p->left and !p->right) or
               (last==p->left) or (last==p->right)
            ){
                Show(p->element);
                last = p;
                St.pop();
            }else{
                if(p->right){
                    St.push(p->right);
                }
                if(p->left){
                    St.push(p->left);
                }
            }
        }
    }

    void insert(const Element& temp){
        ++Size;
        if(root== nullptr){
            root = new Node(temp);
            Size = 1;
        }else{
            Node *it = root, *pre = root;
            while (it != nullptr) {
                pre = it;
                if (_Compare(temp, it->element)) {
                    it = it->left;
                } else {
                    it = it->right;
                }
            }
            auto data = new Node(temp);
            data->father = pre;
            if (_Compare(temp, pre->element)) {
                pre->left = data;
            } else {
                pre->right = data;
            }
        }
    }

    void remove(typename Tree<Element>::iterator &it){
        if(it->PtrNode){Remove(it->PtrNode);}
    }

private:
    struct Node {
        explicit Node(const Element & temp ,Node *f = nullptr, Node *l = nullptr, Node *r = nullptr) :
        father(f), left(l), right(r) ,element(temp){}
        Node *father, *left, *right;
        Element element;
        ~Node()=default;
    };

    __Index Size;
    Node *root;

    void TransPlant(Node *_f, Node *_t) {
        if (_f->father == nullptr) {
            root = _t;
        } else if (_f == _f->father->left) {
            _f->father->left = _t;
        } else {
            _f->father->right = _t;
        }
        if(_t)_t->father = _f->father;
    }

    Node *TreeMin(Node *temp){
        while (temp->left)temp = temp->left;
        return temp;
    }

    void Remove(Node *z){
        Node *y = z;
        if (z->left == nullptr) {
            TransPlant(z, z->right);
        } else if (z->right == nullptr) {
            TransPlant(z, z->left);
        } else {
            y = TreeMin(z->right);
            if (y->father != z) {
                TransPlant(y, y->right);
                y->right = z->right;
                y->right->father = y;
            }
            TransPlant(z, y);
            y->left = z->left;
            y->left->father = y;
        }
        delete z;
    }

};
#define MAX_FILE_SIZE 1048576*10
class HuffmanTree{
private:
   // typedef hash_map;
    typedef std::pair<double,char> Pd;
    struct Node{
        std::pair<double,char>data;
        __Index self;
        int Son[2];
        explicit Node(double ra,char val,int l,int r,__Index S):self(S){
            data.first = ra;
            data.second = val;
            Son[0] = l;
            Son[1] = r;
        }
        Node(const Node&Copy):data(Copy.data.first,Copy.data.second){
            std::copy(Copy.Son,Copy.Son+2,Son);
            self = Copy.self;
        }
        bool operator<(const Node&temp)const {
            return data.first>temp.data.first;
        }
    };
    Vector<Node>Store;////树的结构
    bool *Seq;
    Vector<std::pair<char,std::string> >Code;///字的编码

    char UnCode[8* sizeof(char)];
    std::unordered_map<char,std::string>  Ref;///字的hash表
    __Index Bits;///一个char所占的位
    void Dfs(int cur,int dep){
        if(cur!=-1){
            if(Store[cur].data.second!=-1){
                std::string fic;
                for(int i = 0 ; i < dep ; ++i){
                    fic.push_back(('0'+Seq[i]));
                }
                Code.emplace_back(std::make_pair(Store[cur].data.second,fic));
            }else{
                for(int i = 0 ; i < 2 ; ++i){
                    Seq[dep] = (bool)i;
                    Dfs(Store[cur].Son[i],dep+1);
                }
            }
        }else return;
    }
    void Deal(){
        int cur = Store.back().self;
        Seq = new bool[Store.size()/2+5];
        Code.clear();
        Dfs(Store.back().self,0);
        Ref.clear();
        for(auto &i:Code){
            Ref.insert(i);
        }
        delete []Seq;
    }///建立索引表
    void PreDeal(){
        Bits = 8* sizeof(char);
        for(int i = 0 ; i < Bits ; ++i){
            UnCode[i] = (char)(1<<(Bits-i-1));
        }
    }
    ///对bool型数据进行解码，返回字符集
    void Trans(bool *s,__Index len,std::string&ret){
        bool *e = s + len;
        int cur = Store.back().self;
        while (s!=e){
            if(Store[cur].Son[*s]==-1) {
                ret.push_back(Store[cur].data.second);
                cur = Store.back().self;
            }
            cur = Store[cur].Son[*s];
            ++s;
        }
        if(Store[cur].data.second!=-1)ret.push_back(Store[cur].data.second);
    }

    /**
     * @note 建立huffmanTree的核心代码，只有短短数行
     * @param Info
     */
    void BuildTree(Vector<Pd> &Info) {
        Heap<Node>Pri_Q;///根据Node重载的 "<" 运算符，这是一个小根堆
        __Index order = 0;
        for(auto &it:Info){
            Store.emplace_back(Node(it.first,it.second,-1,-1, order));///Store才是
            Pri_Q.push(Node(it.first,it.second,-1,-1, order));
            ++order;
        }
        while (Pri_Q.size()>1){
            Node first = Pri_Q.top();
            Pri_Q.pop();
            Node second = Pri_Q.top();
            Pri_Q.pop();
            Node temp(first.data.first+second.data.first,-1,first.self,second.self,order);
            Pri_Q.push(temp);
            Store.push_back(temp);
            ++order;
        }
        Store.push_back(Pri_Q.top());
        Deal();
        Seq = nullptr;
        Pri_Q.pop();
    }///输入某文章的统计数据进行建树
public:

    HuffmanTree(){///建立空树
        PreDeal();
        Seq = nullptr;
    }

    /**
     * @param FileLoc 文件路径
     * @note 统计文件中的字符，建立哈夫曼树
     */
    HuffmanTree(const std::string&FileLoc) {///直接用文件建树
        PreDeal();
        std::ifstream inf;
        inf.open(FileLoc, std::ios::in|std::ios::binary);
        long long sum = 0;
        char *str = new char[MAX_FILE_SIZE];
        Vector<std::pair<double,char> >FF;
        std::unordered_map<char,int>cnt;
        std::string article;
        inf.read(str,MAX_FILE_SIZE* sizeof(char));
        for(int i = 0 ; str[i] ; ++i){
            ++cnt[str[i]];
            ++sum;
            article.push_back(str[i]);
        }
        char *S = new char[sum*8];
        for (auto it = cnt.begin(); it != cnt.end(); ++it) {
            FF.emplace_back(std::make_pair(1.0 * (it->second) / sum, it->first));
        }
        BuildTree(FF);
        FF.clear();
        inf.close();
        delete []str;
    }

    ~HuffmanTree()= default;

    /**
     * @param FileLoc 存放压缩文件的位置
     * @param Article 欲压缩的文章
     * @note 将文件压缩
     */
#define DEBUG 32
    void EnCode(const std::string&FileLoc,const std::string &Article){
        std::ofstream ouf;
        ouf.open(FileLoc,std::ios::binary|std::ios::out);
        char *S = new char[(Article.size()+1)];
        int cnt = 0;
        for(auto &i:Article){
            const auto &it = Ref.find(i);
            if(it==Ref.end()) {
                if((int)i==-1)break;
                continue;///发现未甄别字符则继续
            }
            for(auto j:it->second){
                S[cnt/ Bits] = (S[cnt/Bits]<<1)+(j-'0');
                ++cnt;
            }
        }
        ouf<<cnt <<" ";
        ouf.write(S,cnt/Bits+(cnt%Bits!=0));
        ouf.close();
        delete []S;
    }

    /**
     * @param FileLoc 存放压缩文件的位置
     * @param ret 按照当前huffmanTree解压的结果
     * @note 解压文件
     */
    void OutCode(const std::string&FileLoc,std::string &ret){
        std::ifstream inf;
        inf.open(FileLoc,std::ios::binary|std::ios::in);
        if(!inf.is_open())return;
        __Index cnt,Bytes;
        __Index total = 0;
        inf>>cnt;
        Bytes = cnt/Bits;
        char c = (char)inf.get();
        char *str = new char[Bytes+2];
        bool *Read = new bool[cnt];
        inf.read(str,cnt* sizeof(char));
        int i,j = 0,last = 0;
        for(i = 0 ; i < Bytes ; ++i){
            for (j = 0; j < Bits; ++j) {
                Read[total++] = (bool) (str[i] & UnCode[j]);
            }
        }

        last = cnt - total;
        for (j = 0; j < last; ++j) {
            Read[total++] = (bool) (str[i] & UnCode[j + Bits - last]);
        }
        Trans(Read,cnt,ret);
        inf.close();
        delete []str;
        delete []Read;
    }

    /**
     * @note 保存huffmanTree
     * @param FileLoc
     */
    void WriteTree(const std::string&FileLoc){
        std::ofstream ouf;
        ouf.open(FileLoc,std::ios::out);
        ouf<<Store.size()<<"\n";
        for(auto &i:Store){
            ouf<<(int)i.data.second<<" "<<i.Son[0]<<" "<<i.Son[1]<<" "<<i.self<<"\n";
        }
        ouf<<Code.size()<<"\n";
        for(auto &i:Code){
            ouf<<(int)i.first<<" "<<i.second<<'\n';
        }
        ouf.close();
    }

    /**
     * @note 读取huffmanTree
     * @param FileLoc
     */
    void ReadTree(const std::string&FileLoc){
        Code.clear();
        Store.clear();std::ifstream inf;
        inf.open(FileLoc,std::ios::in);
        int _n,temp;double rate;int val;
        int l,r;
        std::string da;
        __Index self;
        inf>>_n;
        for(int i = 0 ; i < _n ; ++i){
            inf>>val;
            inf>>l>>r>>self;
            Store.emplace_back(Node(0.0,(char)val,l,r,self));
        }
        inf>>_n;
        for(int i = 0 ; i < _n ; ++i){
            inf>>val;
            inf>>da;
            Code.emplace_back(std::make_pair((char)val,da));
            Ref.insert(Code.back());
        }
        inf.close();
    }

    Vector<std::pair<char,std::string> >&Result(){///查看字符及其对应的哈夫曼编码
        return Code;
    }

    void CheckCode(char A){
        auto it = Ref.find(A);
        if(it!=Ref.end()){
            puts(it->second.c_str());
        }else {
            puts("Error");
        }
    }

    void ShowTree(){
        printf("Size:%d\n",Code.size());
        for(auto &it:Code){
            printf("[ %c] :%s\n",it.first,it.second.c_str());
        }
    }

};
/** HuffmanTree TestCode
#include <algorithm>
#include <iostream>
#include <cmath>
#include <queue>
#include <map>
#include "../DataStruct/DataStruct.h"
#include <fstream>
#include <bitset>
#include <unordered_map>
using namespace std;
fstream inf,ouf;
vector<int>res;
map<int,int>cnt;
int c;
Vector<pair<double,char > >FF;
bitset<20>s;
string article;
char str[2024*1024*10];
int main() {
    HuffmanTree ReadHuffman;
    HuffmanTree __OriginHuffman("_from.txt");///利用文件建树
    __OriginHuffman.WriteTree("Huffman.txt");///存树
    ReadHuffman.ReadTree("Huffman.txt");  ///读树
    fstream inf;
    inf.open("_from.txt",ios::in);
    int c;
    inf.read(Str, sizeof(Str));
    inf.close();
    __OriginHuffman.EnCode("rar.txt",Str);/// 将"_from.txt"编码，压入"rar.txt"
    str.clear();
    ReadHuffman.OutCode("rar.txt",str);///从"rar.txt"中读取结果
    ofstream ouf;
    ouf.open("_to.txt",ios::out);///存入"_to.txt"
    ouf.write(str.c_str(), sizeof(char)*str.size());
   /// puts(str.c_str());
}//// 1011111 1111101*/