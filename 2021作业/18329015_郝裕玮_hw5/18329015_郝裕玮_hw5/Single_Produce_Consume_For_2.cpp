#include <iostream>
#include <queue>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h> //为使用信号量需要导入的库

using namespace std;

sem_t num;//信号量num代表当前队列里有无资源，防止队列无资源时被消费者占用线程
sem_t t;//信号量t代表当前线程是否被生产者或者消费者占用，防止二者发生冲突，类似于读写锁

int cnt;//用于记录当前队列内资源数

queue<int> resourse;//资源队列

//生产者
void produce(){
    while(true){
        int element;//加入队列的元素

        for(int i=1;i<=100000;i++){
            int j=1;
            j=2;
        }//用于延长函数执行时间，防止线程一直被生产者或消费者占用
        //生产消费交替执行更能体现程序正确性*/

        if(resourse.empty()){
            element=1;//若当前队列为空，则加入值为1的元素
        }
        else{
            element=resourse.back()+1;//不为空则加入值为 队尾+1 的元素
        }  

        sem_wait(&t);//等待信号量t释放再执行接下来的内容
        cnt++;//队列资源数量+1
        resourse.push(element);//将当前元素加入队列
        cout<<"No."<<element<<"已生产！当前队列资源数为："<<cnt<<endl;//打印信息
        sem_post(&t);//释放信号量t,供生产者和消费者占用
        sem_post(&num);//释放信号量num,告诉消费者此时资源队列不再为空

    }
}

//消费者
void consume(){
    while(true){
        int element;//加入队列的元素

        for(int i=0;i<700000;i++){
            int j=1;
            j=2;
        }//用于延长函数执行时间，防止线程一直被生产者或消费者占用
        //生产消费交替执行更能体现程序正确性*/
        
        sem_wait(&num);//等待信号量num释放再执行接下来的内容(资源队列不为空才可以执行消费者函数)
        sem_wait(&t);//等待信号量t释放再执行接下来的内容
        element=resourse.front();//消费元素为当前队列的队头
        resourse.pop();//弹出队头元素
        cnt--;//队列资源数量-1
        cout<<"No."<<element<<"已消费！当前队列资源数为："<<cnt<<endl;//打印信息
        sem_post(&t);//释放信号量t,供生产者和消费者占用
    }
}

int main(){
    //初始化信号量
    sem_init(&num,0,0);
    sem_init(&t,0,1);

    #pragma omp parallel sections//使用section框架
    {
        #pragma omp section
        {
            produce();
        }
        #pragma omp section
        {
            consume();
        }
    }
}
