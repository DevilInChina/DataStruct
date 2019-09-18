/**
 * 矩阵类有关操作 可以把Vector替换为std::vector
 *
 */
#include <functional>
#include <iostream>
#include "Heap.cpp"
template <class T>
class SparseMatrix;

template <class T>
class Matrix {
public:
    friend SparseMatrix<T>;

    explicit Matrix(__Index n,__Index m, const T &Zero = 0,const T&One = 1) :
            O(Zero),I(One), row(n),col(m), total((n+1) * (m)) {
        data = new T[total+1];
        std::fill(data, data + total+1, O);
    }

    Matrix(Matrix &&temp)noexcept:row(temp.row),col(temp.col),O(temp.O),I(temp.I),total(temp.total){
        data = temp.data;
        temp.data = nullptr;
    }

    Matrix(const Matrix<T>&Copy):Matrix(Copy.row,Copy.col,Copy.O,Copy.I){
        std::copy(data,data+total+1,Copy.data);
    }

    explicit Matrix(const SparseMatrix<T>&);

    template <class _T>
    friend Matrix<_T>&&operator*(const Matrix<_T>&,const SparseMatrix<_T>&);

    template <class _T>
    friend Matrix<_T>&&operator*(const SparseMatrix<_T>&,const Matrix<_T>&);

    template <class _T>
    friend Matrix<_T>&&operator*(const Matrix<_T>&M,const Matrix<_T>&N);

    void Input(std::function<void(T&)>
               Indat = [](T&Info)-> void{
                   std::cin>>Info;
               } ){
        for(__Index i = col+1 ; i <= total ; ++i ){
            Indat(*(data + i));
        }
    }

    void Input(std::istream &in ){
        for(int i = 1 ; i <= row ; ++i){
            for(int j = 1 ; j <= col ; ++j){
                in>>operator[](i)[j];
            }
        }
    }

    T *operator[](__Index i) {
        return data + i * col;
    }

    void Fill(const T&Copy){
        std::fill(data,data+total,Copy);
    }

    const T *operator[](__Index i)const {
        return data + i * col;
    }

    void ShowMatrix(std::ostream&out) {
        for(__Index i = 1 ; i <= row ; ++i){
            for(__Index j = 1 ; j <= col ; ++j){
                out<<*(data+i*col+j);
                out<<((j==col)?'\n':' ');
            }
        }
    }

    T &operator()(__Index i) {
        return data[i];
    }

    ~Matrix() {
        delete[]data;
    }

    const T O,I;
private:
    T *data;
    __Index row,col;
    __Index total;
};

template <class T>
class SparseMatrix{
public:
    friend Matrix<T>;
    struct Triple{
        Triple(__Index a,__Index b,T val):i(a),j(b),dat(val){}
        Triple(const Triple &Copy):i(Copy.i),j(Copy.j),dat(Copy.dat){}
        __Index i,j;
        T dat;
    };
    SparseMatrix(__Index n,__Index m,const T& Zero = 0,const T&One = 1):O(Zero),I(One){
        row = n;
        col = m;
        None_O = 0;
        Data.push_back(Triple(__Index (-1),__Index (-1),O));
        RowPos = new __Index[row+1];
    }

    explicit SparseMatrix(const Matrix<T>&);

    SparseMatrix(SparseMatrix<T>&&temp)noexcept:O(temp.O),I(temp.I),Data(std::move(temp.Data)){
        row = temp.row;
        col = temp.col;
        None_O = temp.None_O;
        RowPos = temp.RowPos;
    }

    SparseMatrix(const SparseMatrix&Copy):SparseMatrix(Copy.row,Copy.col,Copy.O,Copy.I){
        std::copy(RowPos,RowPos+row+1,Copy.RowPos);
        for(__Index i = 1; i < Copy.Data.size() ; ++i){
            Data.push_back(Copy.Data[i]);
        }
    }

    template <class _T>
    friend Matrix<_T>&&operator*(const Matrix<_T>&,const SparseMatrix<_T>&);

    template <class _T>
    friend Matrix<_T>&&operator*(const SparseMatrix<_T>&,const Matrix<_T>&);

    template <class _T>
    friend SparseMatrix<_T> &&operator*(const SparseMatrix<_T>& M,const SparseMatrix<_T>& N);

    SparseMatrix<T>&& TransPoseSparseMatrix()const{
        SparseMatrix<T>ret(col,row,O,I);
        if(None_O){
            for(__Index COL = 1 ; COL <= col ; ++COL){
                for(__Index p = 1 ; p <= None_O ; ++p){
                    if(Data[p].j==COL){
                        ret.Data.push_back(Triple(Data[p].j,Data[p].i,Data[p].dat));
                    }
                }
            }
            ret.ReOrder();
        }
        return std::move(ret);
    }

    void ShowMatrix(std::function<void(const T&) >Show
    = [](const T&X)->void{ std::cout<<X; }){
        auto it = Data.begin();///初始化
        ++it;
        if(it==Data.end()){
            it = Data.begin();
        }
        for(__Index i = 1 ; i <= row ; ++i){
            for(__Index j = 1 ; j <= col ; ++j){
                if(i==it->i and j == it->j){
                    Show(it->dat);
                    ++it;
                    if(it == Data.end()) {
                        it = Data.begin();
                    }
                }
                else Show(O);
                std::cout<<(j==col?"\n":" ");
            }
        }
    }

    void ShowList(std::function<void(const T&) >Show
    = [](const T&X)->void{ std::cout<<X; }){
        auto it = Data.begin();
        if(it==Data.end())return;
        ++it;
        while (it!=Data.end()){
            std::cout<<it->i<<" "<<it->j<<" ";
            Show(it->dat);
            std::cout<<"\n";
            ++it;
        }
    }

    void Input(std::function<void(T&)>
               Indat = [](T&date)-> void{
                   std::cin>>date;
               } ){
        T temp(O);
        if(None_O) {
            Data.clear();
            Data.push_back(Triple(__Index (-1),__Index (-1),O));
        }
        None_O = 0;
        for(__Index i = 1; i <= row ; ++i){
            for(__Index j = 1 ; j <= col ; ++j){
                Indat(temp);
                if(temp != O){
                    Data.push_back(Triple(i,j,temp));
                }
            }
        }
        ReOrder();
    }

    void Input(std::istream &in){
        __Index temp;
        in>>temp;
        __Index cur_i ,cur_j;
        T DAT;
        None_O = 0;
        while (temp--){
            in>>cur_i>>cur_j>>DAT;
            if(DAT!=None_O)Data.push_back(Triple(cur_i,cur_j,DAT));
        }
        ReOrder();
    }

    const T O;///字母 'O'作为0元
    const T I;///字母 'I'作为幺元
private:
    __Index row,col,None_O;///行 列 非零元数量
    Vector<Triple>Data;///可变长数组
    __Index *RowPos;
    void ReOrder(){
        None_O = 0;
        auto it = Data.begin();
        ++it;
        for(__Index i = 1; i <= row ; ++i){
            RowPos[i] = None_O+1;
            for(__Index j = 1 ; j <= col ; ++j){
                if(it->i == i and it->j ==j){
                    ++None_O;
                    ++it;
                }
                if(it==Data.end())it = Data.begin();
            }
        }
    }
};

template <class T>
SparseMatrix<T>::SparseMatrix(const Matrix<T> &temp):
        SparseMatrix(temp.row,temp.col,temp.O,temp.I) {
    None_O = 0;
    for(__Index i = 1; i <= row ; ++i){
        RowPos[i] = None_O+1;
        for(__Index j = 1 ; j <= col ; ++j){
            if(temp[i][j] != O){
                Data.push_back(Triple(i,j,temp[i][j]));
                ++None_O;
            }
        }
    }
}

template <class T>
Matrix<T>::Matrix(const SparseMatrix<T> &temp):
        Matrix(temp.row,temp.col,temp.O,temp.I) {
    __Index it = 0;
    if(!temp.Data.empty())it = 1;
    for(__Index i = 1 ; i <= row ; ++i){
        for(__Index j = 1 ; j <= col ; ++j){
            if(i==temp.Data[it].i and j == temp.Data[it].j){
                this->operator[](i)[j] = temp.Data[it].dat;
                ++it;
                if(it == temp.Data.size()) {
                    it = 0;
                }
            }
        }
    }
}

template<class T>
SparseMatrix<T> &&operator*(const SparseMatrix<T>& M,const SparseMatrix<T>& N){
    SparseMatrix<T> *Ans;
    if(M.col!=N.row) return std::move(*(new SparseMatrix<T> (0,0,M.O,M.I)));
    else Ans = new SparseMatrix <T>(M.row,N.col,M.O,M.I);
    if(N.None_O*N.None_O==0)return std::move(*Ans);
    __Index M_row, N_row, Ans_col;
    T *AnsColRes = new T[N.col+2];
    for(M_row = 1 ; M_row <= M.row ; ++M_row){///i from (1 to M_row)
        std::fill(AnsColRes,AnsColRes+N.col+1,M.O);///结果矩阵列叠加初始化清零
        Ans->RowPos[M_row] = Ans->None_O+1;
        __Index tp;///遍历终点代表
        if(M_row < M.row)tp = M.RowPos[M_row+1];///
        else tp = M.None_O+1;///下一行第一个非零元素在三元表中的位置
        for(__Index pos = M.RowPos[M_row] ; pos < tp ; ++pos){///遍历当前 M_row 行中的非零元素
            N_row = M.Data[pos].j;///被乘数矩阵的 列 即是乘数矩阵的对应的 行
            __Index t;
            if(N_row < N.row)t = N.RowPos[N_row+1];
            else t = N.None_O+1;///N矩阵中下一行第一个非零元素在三元表中的位置
            for(__Index X_pos = N.RowPos[N_row] ; X_pos < t ; ++X_pos){
                Ans_col = N.Data[X_pos].j;
                AnsColRes[Ans_col]+=M.Data[pos].dat*N.Data[X_pos].dat;
                /// M.Data[pos].i == M_row  N.Data[X_pos].i == N_row
                /// M.Data[pos].j == N_row  N.Data[X_pos].j == Ans_col
            }
        }
        for(Ans_col = 1 ;Ans_col <= N.col ; ++Ans_col){///j from(1 to N_col)
            if(AnsColRes[Ans_col]!=Ans->O){
                Ans->Data.push_back(typename SparseMatrix<T>::Triple(M_row,Ans_col,AnsColRes[Ans_col]));
                ++Ans->None_O;
            }
        }
    }
    delete []AnsColRes;
    return std::move(*Ans);
}

template <class T>
Matrix<T>&&operator*(const Matrix<T>&M,const Matrix<T>&N){
    Matrix<T>*Ans;
    if(M.col==N.row)Ans = new Matrix<T>(M.row,N.col,M.O,M.I);
    else return std::move(*(new Matrix<T>(0,0,M.O,M.I)));///出错 无法相乘
    for(__Index i = 1; i <= Ans->row ; ++i){
        for(__Index j = 1 ; j <= Ans->col ; ++j){
            for(__Index k = 1 ; k <= M.col ; ++k){
                Ans->operator[](i)[j]+=M[i][k]*N[k][j];
            }
        }
    }
    return std::move(*Ans);
}

template <class T>
Matrix<T>&&operator*(const Matrix<T>&M,const SparseMatrix<T>&N){
    return (std::move(operator*(M,Matrix<T>(N))));
}

template <class T>
Matrix<T>&&operator*(const SparseMatrix<T>&M,const Matrix<T>&N){
    return (std::move(*(new Matrix<T>(M))* N));
}
