#include <iostream>
#include <cstdlib>
#include <time.h>
#include <sys/time.h> //用于计时
#include <cstdio>
#include <cstring>
#include <windows.h> //调用dummy中的Sleep函数
#include <omp.h> //使用openmp时必须包含该库

using namespace std;

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}
//最后一行将us转换为s，统一单位
//该结构体用于计算多线程和串行计算pi的运行时间

void dummy();//题目要求的延迟睡眠函数dummy
void Static();//调度方式为static
void Dynamic();//调度方式为dynamic
void Guided();//调度方式为guided

int main(){
    double start,end;//用于记录开始和结束的时间
    
    GET_TIME(start);
    Static();
    GET_TIME(end);
    cout<<"static调度方式的用时为："<<end-start<<endl;

    GET_TIME(start);
    Dynamic();
    GET_TIME(end);
    cout<<"dynamic调度方式的用时为："<<end-start<<endl;

    GET_TIME(start);
    Guided();
    GET_TIME(end);
    cout<<"guided调度方式的用时为："<<end-start<<endl;
}

void dummy(){
    Sleep(100);//睡眠/延迟时间可修改,单位为ms
}

void Static(){
#pragma omp parallel for num_threads(4) schedule(static)
    for(int i=1;i<=100;i++){
        dummy();
    }
}

void Dynamic(){
#pragma omp parallel for num_threads(4) schedule(dynamic)
    for(int i=1;i<=100;i++){
        dummy();
    }
}

void Guided(){
#pragma omp parallel for num_threads(4) schedule(guided)
    for(int i=1;i<=100;i++){
        dummy();
    }
}
