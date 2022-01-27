#include<stdio.h>
#include<immintrin.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
#define n 1000000

int a[n], b[n],sum1[n],sum2[n];

#define GET_TIME(now) { \
   struct timeval t; \
   gettimeofday(&t, NULL); \
   now = t.tv_sec + t.tv_usec/1000000.0; \
}
//用于计算程序运行时间

//利用AVX指令集进行并行向量相加（包含数据存储）
void AVX(){
    int i=0;
    __m256i a1,b1,c;//__m256i:包含若干个整型数字的向量
    for(i=0;i<=n-1;i+=8){
        a1=_mm256_load_si256((const __m256i*)(a+i));//_mm256_load_si256:从对齐的内存地址加载整形向量
        b1=_mm256_load_si256((const __m256i*)(b+i));
        c=_mm256_add_epi32(a1,b1);//_mm256_add_epi32:对两个整形向量做加法
        _mm256_store_ps((float*)(sum1+i),(__m256)c);//_mm256_store_ps:用于将计算结果等AVX寄存器的数据保存到内存中。
        //将结果存储到sum1数组中
    }
}

//利用AVX指令集进行并行向量相加（不包含数据存储）
void AVX2(){
    int i=0;
    __m256i a1,b1,c;
    for(i=0;i<=n-1;i+=8){
        a1=_mm256_load_si256((const __m256i*)(a+i));
        b1=_mm256_load_si256((const __m256i*)(b+i));
        c=_mm256_add_epi32(a1,b1);
        //这里不包含_mm256_store_ps，即只进行向量加法而不保留结果
    }
}

//串行计算向量相加（包含数据存储）
void Serial(){
    int i=0;
    for(i=0;i<=n-1;i++){
        sum2[i]=a[i]+b[i];//将结果存储到sum2数组中
    }        
}

//串行计算向量相加（不包含数据存储）
void Serial2(){
    int i;
    for(i=0;i<=n-1;i++){
        a[i]+b[i];//仅进行向量加法，不存储结果
    } 
}

int main(){
    int i;
    srand((int)time(0));
    for(i=0;i<=n-1;i++){
        a[i]=rand()%100;
        b[i]=rand()%100;
    }
    //生成随机数组a和b
    int times=10000;//为了放大时间，将AVX向量相加和串行向量相加均重复10000次
    
    double start1,end1,start2,end2,start3,end3,start4,end4;
    GET_TIME(start1);
    while(times--){
        AVX();
    }
    GET_TIME(end1);
    printf("AVX总时间: %f\n",end1-start1);

    times=10000;
    GET_TIME(start2);
    while(times--){
        AVX2();
    }
    GET_TIME(end2);
    printf("AVX数据存储所需时间: %f\n",(end1-start1)-(end2-start2));

    times=10000;
    GET_TIME(start3);
    while(times--){
        Serial();
    }
    GET_TIME(end3);
    printf("串行总时间: %f\n", end3-start3);

    times=10000;
    GET_TIME(start4);
    while(times--){
        Serial2();
    }
    GET_TIME(end4);
    printf("串行数据存储所需时间: %f\n",(end3-start3)-(end4-start4));

    printf("实际加速比: %f\n",(end3-start3)/(end1-start1));
    double t1,t2,w;
    t1=((end1-start1)-(end2-start2))/(end1-start1);
    t2=((end3-start3)-(end4-start4))/(end3-start3);
    w=(t1+t2)/2;
    printf("AVX数据存储时间占比为: %f\n",t1);
    printf("串行数据存储时间占比为: %f\n",t2);
    printf("数据存储时间平均占比为: %f\n",w);
    printf("由Amdahl定律可知，理想情况下的最优加速比为: %f\n",1/(w+(1-w)/8));

    for(i=0;i<n-1;i++){
        if(sum1[i]!=sum2[i]){
            printf("计算错误!\n");
        }
    }
    //检验AVX相加结果是否和串行一致（该部分代码可用于检测AVX指令集应用是否正确）
    return 0;
}