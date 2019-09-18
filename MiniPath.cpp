//
// Created by DevilInChina on 2018/9/20.
//


#include "Tree.cpp"
template <class T>
class MiniPath{///下标从0-n 左右闭区间
public:
    explicit MiniPath(__Index l,const T&Inf,const T&Zero = 0):inf(Inf),GraphSize(l),O(Zero){
        Dis = new T[GraphSize+1];
    }
    ~MiniPath(){
        delete []Dis;
    }
    struct Node{
        Node(__Index v,const T&Dis,int Ne):to(v),dis(Dis),next(Ne){}
        Node(const Node&Copy){
            to = Copy.to;
            dis = Copy.dis;
            next = Copy.next;
        }
        __Index to;
        T dis;
        int next;
    };
    virtual List<__Index >&& GetMiniPath(__Index a,__Index b) = 0;

    const T inf,O;
    __Index GraphSize;
protected:
    T *Dis;
};


template <class T>
class Dijkstra: private MiniPath<T>{
public:
    explicit Dijkstra(__Index n,const T& Inf):MiniPath<T>(n,Inf),end(n+1){
        Head = new int[end];
        pre = new __Index[end];
        cnt = 0;
    }
    void init(){
        std::fill(Head,Head+end,-1);
        __Index i = 0 ,*s = pre;
        __Index *e = pre+end;
        Edge.clear();
        cnt = 0;
        while (s!=e){
            *s = i;
            ++s,++i;
        }
    }

    void AddEdge(__Index From,__Index To,const T&D){
        Edge.push_back(typename MiniPath<T>::Node(To,D,Head[From]));
        Head[From] = cnt++;
    }

    void dijkstra(__Index u,std::function<bool(const T&,const T&)>
    cmp = [](const T&a,const T&b)->bool{
        return a<b;
    }){
        std::function<bool(const std::pair<T,__Index >&,const std::pair<T,__Index >&)>
                CmpHeap=[&](const std::pair<T,__Index >&a,const std::pair<T,__Index >&b)->bool{
            return cmp(b.first,a.first);
        };
        std::fill(MiniPath<T>::Dis,MiniPath<T>::Dis+end,MiniPath<T>::inf);
        MiniPath<T>::Dis[u] = MiniPath<T>::O;
        Heap<std::pair<T,__Index >>Q;
        Q.push(std::make_pair(MiniPath<T>::Dis[u],u));
        while (!Q.empty()){
            __Index From = Q.top().second;
            Q.pop();
            __Index v;
            for(int i = Head[From] ; ~i ; i = Edge[i].next){
                v = Edge[i].to;
                if(MiniPath<T>::Dis[v] > MiniPath<T>::Dis[From]+Edge[i].dis){
                    pre[v] = From;///保存前驱
                    MiniPath<T>::Dis[v] = MiniPath<T>::Dis[From]+Edge[i].dis;
                    Q.push(std::make_pair(MiniPath<T>::Dis[v],v));
                }
            }
        }
    }

    const T&DisTo(__Index a){
        return MiniPath<T>::Dis[a];
    }

    List<__Index >&& GetMiniPath(__Index a,__Index b){
        if(MiniPath<T>::Dis[a]!=MiniPath<T>::O){
            dijkstra(a);
        }
        Stack<__Index >Temp(end+10);
        __Index cur = b;
        List<__Index >ret;
        if(MiniPath<T>::Dis[b] == MiniPath<T>::inf)return std::move(ret);
        Temp.push(cur);
        do{
            cur = pre[cur];
            Temp.push(cur);
        }while (cur!=a);
        return std::move(ret);
    }

    ~Dijkstra(){
        delete []Head;
        delete []pre;
        Head = pre = nullptr;
    }
    const __Index end;
private:
    Vector<typename MiniPath<T>::Node>Edge;
    /// typename MiniPath<T>::Node Edge[10005];
    int *Head;
    __Index cnt;///边的数目
    __Index *pre;
};

template <class T,class Compare = std::less<T> >
class Floyd:private MiniPath<T>{///由于手写矩阵类的特性，结点编号是 [1,n]
public:
    explicit Floyd(__Index L,const T&inf,const T&Zero = 0,const T&One = 1):
    MiniPath<T>(L,inf,Zero),plant(L,L,Zero,One),fromTo(L,L,0,1){
        plant.Fill(inf);
        Min = [&](const T&a,const T&b)->const T&{
            return cmp(a,b)?a:b;
        };
        for(__Index i = 1; i <= MiniPath<T>::GraphSize ; ++i){
            for(__Index j = 1; j <= MiniPath<T>::GraphSize ; ++j){
                fromTo[i][j] = j;
            }
        }
    }

    void Indata(std::istream &in){
        plant.Input(in);
    }

    void AddEdge(__Index a,__Index b,const T&dis){
        plant[a][b] = Min(plant[a][b],dis);
    }

    void floyd(){
        for(__Index k = 1; k <= MiniPath<T>::GraphSize ; ++k){
            for(__Index i = 1; i <= MiniPath<T>::GraphSize ; ++i){
                for(__Index j = 1; j <= MiniPath<T>::GraphSize ; ++j){
                    plant[i][j] = Min(plant[i][j],plant[i][k]+plant[k][j]);
                    if(plant[i][j] == plant[i][k]+plant[k][j]){///松弛成功，或原本相等
                        fromTo[i][j] = fromTo[i][k];
                    }
                }
            }
        }
    }

    void Show(std::ostream&out){
        plant.ShowMatrix(out);
    }

    List<__Index >&& GetMiniPath(__Index a,__Index b){
        List<__Index >ret;
        if(plant[a][b]==MiniPath<T>::inf){
            return std::move(ret);
        }
        __Index x = a;
        ret.push_back(a);
        do{
            x = fromTo[x][b];
            ret.push_back(x);
        }while (x!=b);
        return std::move(ret);
    }

private:
    Matrix<T> plant;
    Matrix<__Index > fromTo;
    Compare cmp;
    std::function <const T&(const T&,const T&)>Min;
};
