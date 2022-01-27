#include<iostream>

using namespace std;

//矩阵一般乘法
void simple(int**a,int**b,int**c,int n){
    int i,j,k;
    for(i=0;i<=n-1;i++){
        for(j=0;j<=n-1;j++){
            for(k=0;k<=n-1;k++){
                c[i][j]+=a[i][k]*b[k][j];//根据矩阵计算公式得出（注意不能直接等于，而应该是累加）
            }
        }
    }
}

int main(){
    int n=512;//矩阵维度，在这里代表矩阵是512*512的
    //因为分治和Strassen的应用场景均是矩阵维度为2的n次方，所以将300*300统一修改为512*512
    int i,j;
    int** a,**b,**c;//声明二维数组指针，为何不直接声明二维数组：int a[n][n]会在Strassen程序中说明

    a=new int*[n];
    b=new int*[n];
    c=new int*[n];
    
    for(i=0;i<=n-1;i++){
        a[i]=new int[n];
        b[i]=new int[n];
        c[i]=new int[n];
    }
    //以上均为创建二维数组的过程

    //给数组a赋值
    for(i=0;i<=n-1;i++){
        for(j=0;j<=j-1;j++){
            a[i][j]=i+1;
        }
    }

    //给数组b赋值
    for(i=0;i<=n-1;i++){
        for(j=0;j<=j-1;j++){
            b[i][j]=j+1;
        }
    }

    //为了与分治和Strassen保持一致（因为这两个程序涉及到递归函数），所以在这里不直接进行运算而是通过调用函数进行运算
    simple(a,b,c,n);
}
