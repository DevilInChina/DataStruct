//
// Created by DevilInChina on 2018/6/11.
//


#include <functional>
/**
 * @note 本文件大多代码不支持stl迭代器作为参数传入，反而很像c语言风格......
 */
typedef unsigned int __Index;
///称这两个函数为比较函数，函数名即相当于指针，将其写入排序算法中，实现代码复用

template <class Element>
const std::function<bool(const Element&,const Element&)>DefaultCmp
= [](const Element&a,const Element&b)->bool{
    return a<b;
};
template <class Element>
std::function<bool(const Element&,const Element&)>_Compare;
template <class Element>
class Sort {
private:

    /**
     * @param s:开始指针 e：结束指针
     * @param cmp 指向函数的指针
     * @note 快速排序:平均时间复杂度 O(n*ln(n))  最坏情况(倒序)O(n^2)
     */
    void static quick_sort(Element *s, Element *e) {///对于左闭右开的区间中的数据(Element)进行排序
///中心思想:递归，分治求解
        Element* i = s;
        Element* j = e;
        if(i==j)return;
        --j;
        if(i==j)return;
        Element key = *i;///选择开始位置为"标记值"
        while (i != j) {
            while (!_Compare<Element>(*j, key) && i < j)j--;///从后往前 *j 不小于 key 说明从j开始后面的元素都>=key，否则结束循环
            while (!_Compare<Element>(key, *i) && i < j)i++;///与上同理
            if (i < j) {                    ///到这里我们找到了位置不对的一组元素
                std::swap(*i, *j);
            }
        }
        *s = *i;///把key放到适当的位置，这时满足key左边的元素均不大于（小于）key，右边元素均不小于（大于）key
        *i = key;
        quick_sort(s, i);///对key左，右侧进行同样
        ++i;
        quick_sort(i, e);
    }

    /**
     * @param s:开始指针 e：结束指针 k：临时储存
     * @note 归并排序:时间复杂度 O(n*ln(n))
     * @note 相较快速排序更加稳定
     * @note 可以通过修改程序在 O(n*ln(n))时间内找出数据的逆序数(传统需要O(n^2))
     */

    void static merge(Element *s, Element *e, Element *k) {
        Element *ori_beg = s;
        Element *copy = k;
        Element *mid = (e - s) / 2 + s;///将mid左右两侧的元素在线性时间内合并为有序的，要求:左右两侧区间内均有序
        Element *i = s, *j = mid + 1;
        while (i <= mid && j <= e) {
            if (_Compare<Element>(*i, *j)) {
                *k = *i;
                ++i, ++k;
            } else {
                *k = *j;
                ++k, ++j;
            }
        }
        while (i <= mid) { *k = *i, ++i, ++k; }
        while (j <= e) { *k = *j, ++j, ++k; }
        while (ori_beg <= e) { *ori_beg = *copy, ++copy, ++ori_beg; }
    }
    void static merge_sort(Element *s, Element *e , Element *store) {
        if (s >= e)return;
        Element *mid = (e - s) / 2 + s;
        merge_sort(s, mid , store);
        ++mid;
        merge_sort(mid, e , store);
        merge(s, e , store);///经过上面的操作我们假设左右两边都已经排序完毕，然后进行归并操作
    }
public:

    void static QuickSort(Element *s, Element *e,
    std::function<bool(const Element&,const Element&)> CMP= DefaultCmp<Element>) {
        _Compare<Element> = CMP;
        quick_sort(s,e);
    }

    void static StableSort(Element *s, Element *e,
    std::function<bool(const Element&,const Element&)> CMP= DefaultCmp<Element>) {
        ///中心思想:分治递归
        _Compare<Element> = CMP;
        auto tmp = new Element[e-s];
        --e;
        merge_sort(s, e , tmp);///左右均为闭区间，这里有讲究
        delete []tmp;
    }

/**
 * @note 参考了 CSDN
 * @note 幂增长复杂度 中等数据规模有较好的表现
 */
    void static ShellSort(Element *s, Element *e,
    std::function<bool(const Element&,const Element&)> CMP= DefaultCmp<Element>) {
        _Compare<Element> = CMP;
        ///和插入排序放在一起理解，我们发现插入排序一次只移动一个单位，所以就更改移动的步长，慢慢减小以达到减少时间开销的目的
        __Index dis_seq = 1;
        __Index MaxGap  = (e - s) / 3;
        Element i, *j, *k ,temp;
        while (dis_seq < MaxGap) {
            dis_seq = dis_seq * 3 + 1;///计算增量区间，有特别的讲究，要求元素非质
        }
        MaxGap = e - s;
        while (dis_seq > 0) {///最后一个递增步长是1，否则很大概率排序不会成功
            for (i = dis_seq; i < MaxGap; ++i) {
                temp = *(s + i);
                j = s + i;
                k = s + dis_seq - 1;
                while (j > k && !_Compare<Element>(*(j - dis_seq), temp)) {
                    *j = *(j - dis_seq);///将*j(tmp)插入到合适位置
                    j -= dis_seq;
                }
                *j = temp;
            }
            dis_seq = (dis_seq - 1) / 3;
        }
    }


    void static BubbleSort(Element *s, Element *e,
    std::function<bool(const Element&,const Element&)> CMP= DefaultCmp<Element>){
        _Compare<Element> = CMP;
        --e;
        Element *i, *j = e;///不过多解释，较传统冒泡有两个特别的优化
        Element *last = e;
        bool notOrder;
        do {
            notOrder = false;/// First optimize ,we don't need to sort if it is ordered.
            for (i = s; i < j; ++i) {
                if (_Compare<Element>(*(1 + i), *i)) {
                    std::swap(*i, *(i + 1));
                    notOrder = true;
                    last = i;
                }
            }
            j = last;/// Second optimize ,we don't need to sort if the past sequence are order.
        } while (notOrder);
    }

    void static SelectionSort(Element *s, Element *e,
    std::function<bool(const Element&,const Element&)> CMP= DefaultCmp<Element>){
        _Compare<Element> = CMP;
        Element *i, *j, *mark;///正常思维下的排序
        Element key;
        for (i = s; i < e; ++i) {
            key = *i;
            mark = i;
            for (j = i + 1; j < e; ++j) {
                if (_Compare<Element>(*j, key)) {
                    key = *j;
                    mark = j;
                }
            }
            if (key != *i)std::swap(*i, *mark);
        }
    }

    void static InsertionSort(Element *s, Element *e,
    std::function<bool(const Element&,const Element&)> CMP= DefaultCmp<Element>){
        _Compare<Element> = CMP;
        Element *i, *j, key;///和希尔(shell)排序一样
        for (i = s + 1; i != e; ++i) {
            j = i;
            key = *i;
            while (j > s && !_Compare<Element>(*(j - 1), key)) {
                *j = *(j - 1);
                j--;
            }
            *j = key;
        }
    }
};

/**
 * n(ln(n))
 * 1 快速排序
 * 2 归并排序
 * 介于线性和二次方之间
 * 3 希尔排序
 * 二次方复杂度
 * 4 插入排序
 * 5 选择排序
 * 5 冒泡排序
 */
/*****************
TEST one:
1000000
Case #1:       187.0 ms
Case #2:       188.0 ms
Case #3:       391.0 ms

TEST two:
10000000
Case #1:      4172.0 ms
Case #2:      2136.0 ms
Case #3:     11986.0 ms

TEST three:
10000
Case #1:         0.0 ms
Case #2:         0.0 ms
Case #3:         0.0 ms
Case #4:        62.0 ms
Case #5:       109.0 ms
Case #6:       344.0 ms

TEST four:
100000
Case #1:        15.0 ms
Case #2:        15.0 ms
Case #3:        32.0 ms
Case #4:      6938.0 ms
Case #5:     10611.0 ms
Case #6:     35866.0 ms
*****************/
