//
// Created by DevilInChina on 2018/9/18.
//

#include "Queue_Stack.cpp"
#include "Matrix.cpp"


template <class T>
class Graphic {///下标从1开始
public:
    explicit Graphic(unsigned int n, const T &Zero = 0,const T& One = 1) : map(n, n,Zero,One), size(n) {
        to = new List<__Index >[size+1];
        in_degree = new __Index[n+1];
        out_degree = new __Index[n+1];
        vis = new int[n+1];
        std::fill(in_degree, in_degree + n+1, 0);
        std::fill(out_degree, out_degree + n+1, 0);
    }

    bool AddEdge(__Index From, __Index To, const T &dis) {
        if(From and To and From <= size and To <= size){}
        else return false;
        map[From][To] += dis;
        ++in_degree[To];
        ++out_degree[From];
        to[From].push_back(To);
        return true;
    }


    void Initiate(std::function<void(T&)>
                  DataIn = [](T&_in)->void{
                      std::cin>>_in;
                  }){
        __Index tol = size*size;
        for(__Index i = 1 ; i <= size ; ++i){
            for(__Index j = 1 ; j <= size ; ++j){
                DataIn(map[i][j]);
                if(map[i][j]!=map.O){
                    to[i].push_back(j);
                    ++in_degree[j];
                    ++out_degree[i];
                }
            }
        }
    }

    void DestoryEdge(__Index From, __Index To){
        map[From][To] = map.O;
        auto it = to[From].find(To);
        if(it!=to[From].end())to[From].erase(it);
    }

    ~Graphic() {
        delete[]to;
        delete[]in_degree;
        delete[]out_degree;
        to = nullptr;
        in_degree = out_degree = nullptr;
    }

    void Dfs(__Index u, std::function<void(__Index)> show = [](__Index a) -> void {}) {
        std::fill(vis, vis + size+1, 0);
        Show_way = show;
        dfs(u);
    }

    void Bfs(__Index u, std::function<void(__Index)> show = [](__Index a) -> void {}) {
        std::fill(vis, vis + size, 0);
        Show_way = show;
        vis[u] = 1;
        ListQueue<__Index> que;
        que.push(u);
        while (!que.empty()) {
            __Index from = que.front();
            Show_way(from);
            que.pop();
            for (auto _to = to[from].begin(); _to != to[from].end(); ++_to) {
                if (!vis[(*_to)]) {
                    que.push((*_to));
                    vis[(*_to)] = vis[from] + 1;
                }
            }
        }
        std::cout << "Bfs Layers from " << u << " are as blew";
        for (__Index i = 1; i <= size; ++i) {
            if (!((i-1) % 4))std::cout << "\n";
            std::cout << "[V" << i << " : " << vis[i] << " ]";
        }
        std::cout << "\n";
    }

    void Show_Degree() {
        std::cout << "\n    in   out\n";
        for (__Index i = 1; i <= size; ++i) {
            std::cout << i << "   " << in_degree[i] << "    " << out_degree[i] << "\n";
        }
    }

    void ShowMatrix(std::ostream&out){
        map.ShowMatrix(out);
    }

    void ShowAdjoiningList(std::ostream&out){
        for(__Index i = 1 ; i <= size ; ++i){
            to[i].sort();
            for(auto it:to[i]){
                out<<i<<" "<<it<<" ";
                out<<map[i][it];
                out<<"\n";
            }
        }
    }

    bool HaveEdge(__Index a,__Index b){
        return map[a][b]!=map.O;
    }

    T &dis(int from,int to){///直接修改两点间距离，不建议直接使用
        return map[from][to];
    }

    List<List<__Index > >&&Path(__Index a,__Index b){///计算a到b的所有简单路径
        auto ret = new List<List<__Index > >();
        std::fill(vis,vis+size+1,0);
        auto path(new __Index[size+1]);
        path[0] = a;
        vis[a] = 1;
        dfs(a,b,path,ret,1);
        delete []path;
        return std::move(*ret);
    }

private:
    Matrix<T> map;
    List<__Index> *to;
    __Index size;
    __Index *in_degree;
    __Index *out_degree;
    std::function<void(__Index)> Show_way;
    int *vis;

    void dfs(__Index u,__Index v,__Index *st,List<List<__Index > >*const s,__Index dep){
        if(u==v){
            s->push_back(List<__Index>());
            for(__Index i = 0 ; i < dep ; ++i){
                s->back().push_back(st[i]);
            }
            return;
        }
        for(auto it = to[u].begin() ; it != to[u].end() ; ++it){
            if(!vis[(*it)]){
                st[dep] = (*it);
                vis[st[dep]] = 1;
                dfs(st[dep],v,st,s,dep+1);
                vis[st[dep]] = 0;
            }
        }
    }

    void dfs(__Index u) {
        vis[u] = true;
        Show_way(u);
        for (auto it = to[u].begin(); it != to[u].end(); ++it) {
            if (!vis[(*it)]) {
                dfs((*it));
            }
        }
    }

};
