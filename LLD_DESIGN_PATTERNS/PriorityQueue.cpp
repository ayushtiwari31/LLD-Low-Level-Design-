#include<bits/stdc++.h>
using namespace std;


template<typename T>
class PQ{
    vector<T>pq;
    bool isMinHeap;

    bool compare(int par,int child){
        if(!isMinHeap){
            return pq[child]>pq[par];
        }else{
            return pq[child]<pq[par];
        }
    }

    public:
    PQ(bool isMinHeap=false):isMinHeap(isMinHeap){};

    void push(T num){
        pq.push_back(num);
        int sz=pq.size();
        int i=sz-1;
        while(i>0){
            int j=(i-1)/2;
            // pq[i]>pq[j]
            if(compare(i,j)){
                swap(pq[i],pq[j]);
                i=j;
            }else{
                break;
            }
        }
    }

    T top(){
        if (pq.empty()) {
            throw runtime_error("Priority Queue is empty!");
        }
        return pq[0];
    }

    void pop(){
        swap(pq[0],pq[pq.size()-1]);
        pq.pop_back();
        int i=0;
        
        while(true){
            int left=2*i+1;
            int right=2*i+2;
            int largest=i;
            // pq[left]>pq[largest]
            if(left<pq.size() && compare(largest,left)){
                largest=left;
            }
            // pq[right]>pq[largest]
            if(right<pq.size() && compare(largest,right)){
                largest=right;
            }

            if(largest!=i){
                swap(pq[i],pq[largest]);
                i=largest;
            }else{
                break;
            }
        }
    }

    void print_pq(){
        for(auto it:pq){
            cout<<it<<" ";
        }
        cout<<endl;
    }
};

int main()
{
    PQ<int> pq;

    pq.push(1);
    pq.push(23);
    pq.print_pq();
    pq.push(50);
    pq.print_pq();
    pq.pop();
    pq.print_pq();

    return 0;

}