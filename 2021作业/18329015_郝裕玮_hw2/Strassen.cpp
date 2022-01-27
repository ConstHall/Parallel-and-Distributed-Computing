#include <iostream>

using namespace std;

//矩阵加法
void add(int** a,int** b,int** c,int n){
    int i,j;
    for(i=0;i<=n-1;i++){
        for(j=0;j<=n-1;j++){
            c[i][j]=a[i][j]+b[i][j];
        }
    }
}

//矩阵减法
void sub(int** a,int** b,int** c,int n){
    int i,j;
    for(i=0;i<=n-1;i++){
        for(j=0;j<=n-1;j++){
            c[i][j]=a[i][j]-b[i][j];
        }
    }
}

//矩阵乘法
void mul(int** a,int** b,int** c,int n){
    int i,j,k;
    for(i=0;i<=n-1;i++){
        for(j=0;j<=n-1;j++){
            c[i][j]=0;//因为乘法需要累加，所以要先清空数组c原先的数据
        }
    }

    for(i=0;i<=n-1;i++){
        for(j=0;j<=n-1;j++){
            for(k=0;k<=n-1;k++){
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
}

//Strassen算法实现
void strassen(int **a,int **b,int **c,int n){
    int i,j;
    if(n<=64){
        mul(a,b,c,n);
        //查阅资料可知，最优的界限值在32～128之间，所以这里设置为当n缩小至64时不再需要继续四等分，直接进行普通矩阵乘法即可
        //继续递归分治会导致Strassen算法的效率降低
    }
    else{
        int** a11,**a12,**a21,**a22;      
        int** b11,**b12,**b21,**b22;       
        int** c11,**c12,**c21,**c22;
        
        int** P1,**P2,**P3,**P4,**P5,**P6,**P7;
        int** a_final,**b_final;

        a11=new int*[n/2];
        a12=new int*[n/2];
        a21=new int*[n/2];
        a22=new int*[n/2];
        
        b11=new int*[n/2];
        b12=new int*[n/2];
        b21=new int*[n/2];
        b22=new int*[n/2];

        c11=new int*[n/2];
        c12=new int*[n/2];
        c21=new int*[n/2];
        c22=new int*[n/2];
        
        P1=new int*[n/2];
        P2=new int*[n/2];
        P3=new int*[n/2];
        P4=new int*[n/2];
        P5=new int*[n/2];
        P6=new int*[n/2];
        P7=new int*[n/2];

        a_final=new int* [n/2];
        b_final=new int* [n/2];

        for(i=0;i<=n/2-1;i++){
            a11[i]=new int[n/2];
            a12[i]=new int[n/2];
            a21[i]=new int[n/2];
            a22[i]=new int[n/2];
            
            b11[i]=new int[n/2];
            b12[i]=new int[n/2];
            b21[i]=new int[n/2];
            b22[i]=new int[n/2];
            
            c11[i]=new int[n/2];
            c12[i]=new int[n/2];
            c21[i]=new int[n/2];
            c22[i]=new int[n/2];

            P1[i]=new int[n/2];
            P2[i]=new int[n/2];
            P3[i]=new int[n/2];
            P4[i]=new int[n/2];
            P5[i]=new int[n/2];
            P6[i]=new int[n/2];
            P7[i]=new int[n/2];

            a_final[i]=new int[n/2];
            b_final[i]=new int[n/2];
        }

        //根据原矩阵与四个子矩阵的位置关系将值赋给A和B的四个子矩阵
        for(i=0;i<=n/2-1;i++){
            for (j=0;j<=n/2-1;j++){
                a11[i][j]=a[i][j];
                a12[i][j]=a[i][j+n/2];
                a21[i][j]=a[i+n/2][j];
                a22[i][j]=a[i+n/2][j+n/2];

                b11[i][j]=b[i][j];
                b12[i][j]=b[i][j+n/2];
                b21[i][j]=b[i+n/2][j];
                b22[i][j]=b[i+n/2][j+n/2];
            }
        }
        //以上均为声明新数组a11,a12,a21,a22,b11,b12,b21,b22,c11,c12,c21,c22
        //以及P1,P2,P3,P4,P5,P6,P7,a_final,b_final

        //P1=A11*(B12-B22)
        sub(b12,b22,b_final,n/2);
        strassen(a11,b_final,P3,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）

        //P2=(A11+A12)*B22
        add(a11,a12,a_final,n/2);
        strassen(a_final,b22,P5,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）

        //P3=(A21+A22)*B11
        add(a21,a22,a_final,n/2);
        strassen(a_final,b11,P2,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）

        //P4=A22*(B21-B11)
        sub(b21,b11,b_final,n/2);
        strassen(a22,b_final,P4,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）

        //P5=(A11+A22)*(B12+B22)
        add(a11,a22,a_final,n/2);
        add(b11,b22,b_final,n/2);                
        strassen(a_final,b_final,P1,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）

        //P6=(A12-A22)*(B21+B22)
        sub(a12,a22,a_final,n/2);
        add(b21,b22,b_final,n/2);
        strassen(a_final, b_final, P7,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）

        //P7=(A11-A21)*(B11+B12)
        sub(a11,a21,a_final,n/2);
        add(b11,b12,b_final,n/2);
        strassen(a_final,b_final,P6,n/2);//代入strassen继续递归，会在最底层进行乘法运算（调用mul函数）


        //C11=P5+P4-P2+P6;
        add(P5,P6,a_final,n/2);
        sub(P4,P2,b_final,n/2);
        add(a_final,b_final,c11,n/2);

        //C12=P1+P2;
        add(P1,P2,c12,n/2);

        //C21=P3+P4;
        add(P3,P4,c21,n/2);

        //C22=P5+P1-P3-P7;
        sub(P5,P3,a_final,n/2);
        sub(P1,P7,b_final,n/2);
        add(a_final,b_final,c22,n/2);

        //根据四个子矩阵与原矩阵的位置关系将值赋给数组c
        for(i=0;i<=n/2-1;i++){
            for(j=0;j<=n/2-1;j++){
                c[i][j]=c11[i][j];
                c[i][j+n/2]=c12[i][j];
                c[i+n/2][j]=c21[i][j];
                c[i+n/2][j+n/2]=c22[i][j];
            }
        }
    }

}


int main()
{
    int n=512;//矩阵维度，在这里代表矩阵是512*512的
    int i,j;
    int** a,**b,**c;//声明二维数组指针

    a=new int *[n];
    b=new int *[n];
    c=new int *[n];
    
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

    //初始化数组c
    for(i=0;i<=n-1;i++){
        for(j=0;j<=j-1;j++){
            c[i][j]=0;
        }
    }

    strassen(a,b,c,n);
}