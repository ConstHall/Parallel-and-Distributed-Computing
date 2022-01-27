#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<pthread.h>
#include<semaphore.h>
#include<sys/time.h>

const int MAX_THREADS = 1024;//最大线程数

long thread_count;//线程数量
long long n;//迭代项数
double sum;//公式最终的总和（多线程计算pi的估计量）

sem_t sem;//信号量

void* Thread_sum(void* rank);

int main(int argc, char* argv[]) {
   long       thread;  /* 在64位系统中使用long */
   pthread_t* thread_handles;
   //pthread_t 用于声明线程ID
   double start, finish, elapsed;

   /* 可选择在这里确定公式项数和线程数量 */
   n = 10000;
   thread_count = 4;

   thread_handles = (pthread_t*) malloc (thread_count*sizeof(pthread_t));//创建线程数组
   sem_init(&sem, 0, 1);//初始化信号量
   sum = 0.0;//最终的总和初始化为0

   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,Thread_sum, (void*)thread);//创建线程
      //第一个参数为指向线程标识符的指针。
      //第二个参数用来设置线程属性。
      //第三个参数是线程运行函数的起始地址。
      //最后一个参数是运行函数的参数。

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
      //函数pthread_join用来等待一个线程的结束。
      //第一个参数为被等待的线程标识符，第二个参数为一个用户定义的指针，它可以用来存储被等待线程的返回值。
      //这个函数是一个线程阻塞的函数，调用它的线程将一直等待到被等待的线程结束为止
      //当函数返回时，被等待线程的资源被收回。
      //也就是说主线程中要是加了这段代码，就会在该代码所处的位置卡住，直到这个线程执行完毕才会继续往下运行。

   sum = 4.0*sum;//1/4*pi*4=pi（公式计算的是1/4*pi）
   printf("With n = %lld terms,\n", n);//公式项数
   printf("   Our estimate of pi = %.15f\n", sum);//多线程估计的pi值
   printf("                   pi = %.15f\n", 4.0*atan(1.0));//arctan计算pi值

   sem_destroy(&sem);//释放信号量
   free(thread_handles);//释放线程数组
   return 0;
}  /* main */

/*------------------------------------------------------------------*/
void* Thread_sum(void* rank) {
   long my_rank = (long) rank;
   //如果指针类型的大小和表示进程编号的整数类型不同，在编译时就会受到警告
   //在我们使用的机器上，指针类型64位，int类型32位
   //为了避免警告，所以我们用long型替换int型
   double my_sum = 0.0;//每个线程内部的和（最终要汇总到全局变量sum）
   /*******************************************************************/
   double flag;//用于确定公式中每项的正负
   long long i;
   long long group_num=n/thread_count;//计算每个线程中需要分配的公式项数（尽可能平均）
   long long group_first_i=group_num*my_rank;//计算每个线程中开始累加的第一项
   long long group_last_i=group_first_i+group_num;//计算每个线程中需要累加的最后一项
   if(group_first_i%2==0){
      flag=1.0;
   }
   else{
      flag=-1.0;
   }
   for(i=group_first_i;i<=group_last_i-1;i++){//每个线程内部开始累加
      my_sum+=flag/(2*i+1);//1/4pi的计算公式
      flag*=-1;//改变下一项的正负
   }
   sem_wait(&sem);//阻塞线程，等待获取信号量
   sum+=my_sum;//获取信号量后可将当前线程的和累加进入全局变量sum
   sem_post(&sem);//释放信号量
   /******************************************************************/
   return NULL;
}  /* Thread_sum */



