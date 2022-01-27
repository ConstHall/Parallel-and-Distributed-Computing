#include <iostream>
#include <fstream> //用于文件读取
#include <cstdlib>
#include <sys/time.h> //用于计时
#include <cstdio>
#include <cstring>
#include <omp.h> //使用openmp时必须包含该库
using namespace std;

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}
//最后一行将us转换为s，统一单位

//OpenMP并行函数
void Parallel(int thread_count,int row,int *col_pointer,int *row_pointer,double *value,double *vec,double *ans);
//串行函数
void Serial(int thread_count,int row,int *col_pointer,int *row_pointer,double *value,double *vec,double *ans);

int main() {
    
    int thread_count=4;//这里可修改线程数

    ifstream fin;
    fin.open("C:\\Users\\93508\\Desktop\\bcsstk15.rsa");//读取文件

    string text;//用于存储无关信息
    for(int i=1;i<=2;i++){
        getline(fin,text);//读取文件的前2行无关信息
    }

    int row,col,num;//row:矩阵行数;col:矩阵列数;num:矩阵非零元素个数
    fin>>text>>row>>col>>num>>text;//第3行中间3个数分别为row,col,num
    //打印矩阵信息
    cout<<endl;
    cout<<"矩阵规模："<<row<<"*"<<col<<endl;
    cout<<"矩阵非零元素个数："<<num<<endl;

    for(int i=1;i<=2;i++){
        getline(fin,text);//读取第4行无关信息(需要读2次,因为第3行结尾有个回车/终止符,只读1次会导致读取数据时读入字符串)
    }

    int *row_pointer=(int*)malloc(sizeof(int)*(row+1));
    //row_pointer数组:保存矩阵每行第1个非零元素在value中的索引,大小为row+1
    for(int i=0;i<=row;i++){
        fin>>row_pointer[i];//根据空格依次读入每个值
        row_pointer[i]--;//文件中的矩阵从(1,1)开始,所以需要对所有值-1
    }

    int *col_pointer=(int*)malloc(sizeof(int)*num);
    //col_pointer数组:保存value数组中每个元素的列索引,大小为num
    for(int i=0;i<=num-1;i++){
        fin>>col_pointer[i];//根据空格依次读入每个值
        col_pointer[i]--;//文件中的矩阵从(1,1)开始,所以需要对所有值-1
    }

    double *value=(double*)malloc(sizeof(double)*num);
    //value数组:按顺序保存矩阵所有的非零元素(按从上往下，从左往右的行遍历方式访问元素)
    for(int i=0;i<=num-1;i++){
        fin>>value[i];//根据空格依次读入每个值
    }

    double *vec=(double*)malloc(sizeof(double)*col);
    //vec数组:记录用于和矩阵相乘的列向量,矩阵规模为col*1 
    for(int i=0;i<=row-1;i++){
        vec[i]=2;//为了计算方便,所有元素的值均设为2
    }

    double *ans=(double*)malloc(sizeof(double)*row);
    //ans数组:用于存储矩阵和向量的相乘结果
    memset(ans,0,sizeof(double)*row);//结果数组的元素全部初始化为0
    
    double start1,end1,start2,end2;
    int t=1000;//为了放大时间便于比较,设置串行和并行均循环运算t次(次数t视矩阵规模大小而定)

    cout<<"线程数："<<thread_count<<endl;
    cout<<"循环次数："<<t<<"次"<<endl;
    GET_TIME(start1);//得到串行计算运行的开始时间
    while(t--){
        Serial(thread_count,row,col_pointer,row_pointer,value,vec,ans);//调用串行函数
    }
    GET_TIME(end1);//得到串行计算运行的结束时间
    cout<<"串行时间："<<end1-start1<<endl;//输出串行时间

    t=1000;//将t重置
    GET_TIME(start2);//得到并行计算运行的开始时间
    while(t--){
        Parallel(thread_count,row,col_pointer,row_pointer,value,vec,ans);//调用并行函数
    }
    GET_TIME(end2);//得到并行计算运行的结束时间
    cout<<"并行时间："<<end2-start2<<endl;//输出并行时间
    cout<<"加速比: "<<(end1-start1)/(end2-start2)<<endl<<endl;//输出加速比

    fin.close();//关闭文件
    system("pause");
    return 0;

}

void Parallel(int thread_count,int row,int *col_pointer,int *row_pointer,double *value,double *vec,double *ans)
{
#pragma omp parallel for num_threads(thread_count) //由于内层循环不可拆分(含累加)且外层各循环之间互不影响
    //所以可将外层循环用omp进行并行化 
    for(int i=0;i<=row-1;i++){
        for(int j=row_pointer[i];j<=row_pointer[i+1]-1;j++){//外层循环遍历次数代表第i行的非零元素个数
            int pos=col_pointer[j];//定位第i行非零元素的列坐标
            ans[i]+=value[j]*vec[pos];//将该元素与列向量的对应位置相乘，并累加到结果向量的对应位置上
        }
    }   
}

void Serial(int thread_count,int row,int *col_pointer,int *row_pointer,double *value,double *vec,double *ans)
{
    for(int i=0;i<=row-1;i++){
        for(int j=row_pointer[i];j<=row_pointer[i+1]-1;j++){//外层循环遍历次数代表第i行的非零元素个数
            int pos=col_pointer[j];//定位第i行非零元素的列坐标
            ans[i]+=value[j]*vec[pos];//将该元素与列向量的对应位置相乘，并累加到结果向量的对应位置上
        }
    }
}


