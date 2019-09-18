


#include <iostream>
#include <cstring>
#include <set>
#include "DataStruct.h"
using namespace std;
__Index operator%(const std::string&s,__Index mod){
    __Index ret = 0;
    for(auto c:s){
        ret += c;
        ret+=(ret<<10);
        ret^=(ret>>6);
    }
    ret+=(ret<<3);
    ret^=(ret>>11);
    ret+=(ret<<15);
    return ret;
}
Floyd<int>SIX(2,0x3f3f3f3f);
int main(){
    freopen("data.txt","r",stdin);
    SIX.Indata(cin);
    SIX.Show(cout);
    SIX.floyd();
    SIX.Show(cout);
///    HuffmanTree res(sl);

}
/*
 *
 1 0 1
 0 1 3
 1 1 0

 3 0 0
 0 1 0
 0 0 1



1 1 1
1 2 2
1 3 3
2 1 4
2 2 5
2 3 6
3 1 7
3 2 8
3 3 9


1 1 1
1 2 2
1 3 3
2 2 1
2 3 2
3 3 3
 */