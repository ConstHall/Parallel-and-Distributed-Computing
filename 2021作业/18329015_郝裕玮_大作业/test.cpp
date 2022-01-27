#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stack>
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

//命令行编译: g++ -fopenmp gauss.cpp
//求解Ax=b

//A矩阵生成
void matGene(double *A, int size){
    for(int i = 0; i <= size-1; i++){
        for(int j = 0; j <= size-1; j++){
            //将A[i][j]存储到一维数组A[]中
            A[i * size + j] = rand() % 10; 
        }
    }
}

//b向量生成
void vecGene(double *b, int size){
    for (int i = 0; i <= size-1; i++){
        b[i] = rand() % 5; //A[i]
    }
}

//展示矩阵A
void matShow(double *A, int size){
    //按行打印矩阵
    for (int i = 0; i <= size-1; i++){
        for (int j = 0; j <= size-1; j++){
            cout << A[i * size + j] << " "; 
        }
        cout << endl;
    }
}

//展示向量b
void vecShow(double *b, int size){
    for (int i = 0; i < size; i++){
        cout << b[i] << endl;
    }
}

int main() {
    //设置矩阵(n*n)和向量规模(n*1)
    int n = 1000;
    //初始化
    double* A = new double[n * n + 1];
    double* b = new double[n + 1];
    int* serial_num = new int[n + 1];

    //随机生成矩阵A和向量b
    srand(time(NULL));
    matGene(A, n);
    vecGene(b, n);

    //用于展示初始化矩阵A和向量b
    //cout << "A = " << endl;
    //matShow(A, n);
    //cout << "b = " << endl;
    //vecShow(b, n); 


    for(int i = 0; i < n; i++){
        serial_num[i] = -1;
    }

    stack<int> s1;
    double start,end;

    //记录计算开始时间
    GET_TIME(start);
    //设置omp并行线程数量
    int num_threads = 4;

    //将增广矩阵转化为上三角矩阵
    for(int j = 0; j < n; j++){
        double max_coff = 0;
        int max_index;
        for(int i = 0; i < n; i++){
            if(serial_num[i] == -1 && abs(A[i * n + j]) > abs(max_coff)){
                max_coff = A[i * n + j];
                max_index = i;
            }
        }
        serial_num[max_index] = j;
        s1.push(max_index);
        #pragma omp parallel for num_threads(num_threads)
        for(int i = 0; i < n; i++){
            if(serial_num[i] == -1){
                double tmp_coff = A[i * n + j] / A[max_index * n + j];
                A[i * n + j] = 0;
                for(int k = j + 1; k < n; k++){
                    A[i * n + k] -= tmp_coff * A[max_index * n + k];
                }
                b[i] -= tmp_coff * b[max_index];
            }
        }
    }

    //对上三角矩阵进行回代求解
    double* result = new double[n + 2];
    for(int j = n - 1; j >= 0; j--){
        int this_index = s1.top();
        s1.pop();
        serial_num[this_index] = -1;
        #pragma omp parallel for num_threads(num_threads)
        for(int i = 0; i < n; i++){
            if(serial_num[i] != -1){
                double tmp_coff = A[i * n + j] / A[this_index * n + j];
                A[i * n + j] = 0;
                b[i] -= tmp_coff * b[this_index];
            }
        }
        b[this_index] /= A[this_index * n + j];
        A[this_index * n + j] = 1;
        result[j] = b[this_index];
    }
    GET_TIME(end);
    //可展示解向量x
    //cout << "x = " << endl;
    //vecShow(result, n);

    //输出并行运算时间
    cout << "Time: " << end-start << endl;
    
    return 0;
}

