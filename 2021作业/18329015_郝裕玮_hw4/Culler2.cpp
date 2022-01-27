#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <sys/time.h>
#include <time.h>
using namespace std;

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}
////最后一行将us转换为s，统一单位
//该结构体用于计算并行计算和串行计算的运行时间

void Serial(int **a,int*b,int*c,int size){//串行计算
    int i,j;
    for(i=0;i<=size-1;i++){
        for(j=0;j<=size-1;j++){
            c[i]+=a[i][j]*b[j];
            //将矩阵a(1000*1000)第i行的元素与矩阵b(1000*1)进行点乘即可得到结果矩阵c的第i行元素
        }
    }
}

void Parallel(int **a,int*b,int*c,int size,int thread_count){
    int i,j,cnt,div;
    div=10;//设置行数的合并数量
#  pragma omp parallel for num_threads(thread_count) default(none) \
      private(i,j,cnt) shared(a,b,c,size,div) 
    //设置循环迭代的i,j,cnt为私密变量
    //设置a,b,c数组,数组大小size和行数合并数量div为各线程之间的共享变量
    for(cnt=0;cnt<=size-1;cnt+=div){//a矩阵的每div行合并到一起与矩阵b进行点乘运算，而不再单独计算a矩阵的每行
        for(i=cnt;i<=cnt+dvi-1;i++){//计算a矩阵当前的div行(cnt代表组数，div代表组内部的行数)
            for(j=0;j<=size-1;j++){
                c[i]+=a[i][j]*b[j];
            }      
        }
    }
}

int main(){
    int size=1000;//数组大小(可修改)
    int i,j;
    int thread_count=4;//并行线程数(可修改)
    
    //a,b,c数组的初始化
    int** a;
    a=new int*[size];
    for(i=0;i<=size-1;i++){
        a[i]=new int[size];
    }
    int *b;
    b=new int[size];
    int *c;
    c=new int[size];

    srand(time(NULL));//用时间初始化随机数生成种子
    //对a,b,c数组进行随机初始化(a,b数组的每个元素均为0-9的随机整数，c数组初始化为全0)
    for(i=0;i<=size-1;i++){
        for(j=0;j<=size-1;j++){           
            a[i][j]=rand()%10;
        }
    }
    for(i=0;i<=size-1;i++){
        b[i]=rand()%10;
    }
    for(i=0;i<=size-1;i++){
        c[i]=0;
    }

    double t1,t2,t3,t4;

    //记录串行计算时间
    GET_TIME(t1);
    Serial(a,b,c,size);
    GET_TIME(t2);
    cout<<"串行计算时间为："<<t2-t1<<"秒"<<endl;

    //将结果数组c重新全部初始化为0
    for(i=0;i<=size-1;i++){
        c[i]=0;
    }

    //记录并行计算时间
    GET_TIME(t3);
    Parallel(a,b,c,size,thread_count);
    GET_TIME(t4);
    cout<<"并行计算时间为："<<t4-t3<<"秒"<<endl;

    //计算加速比
    cout<<"加速比为："<<(t2-t1)/(t4-t3)<<endl;

    return 0;
}