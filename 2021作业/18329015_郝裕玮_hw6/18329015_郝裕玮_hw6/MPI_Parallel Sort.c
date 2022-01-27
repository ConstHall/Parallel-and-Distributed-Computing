#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

//Merge函数和Merge_Sort函数用于归并排序
 
/*
  函数功能：合并
  函数参数说明：
  arr: 目标数组
  start: 待合并段的开始下标
  mid: 待合并段的中间下标
  end: 待合并段的结束下标
*/
void Merge(int* arr, int start, int mid, int end)
{
    int len_l, len_r; //左右待合并区间的长度
    len_l = mid - start + 1;
    len_r = end - mid;
    int l[len_l], r[len_r]; //两个临时数组，分别保存待合并的两个区间
    memcpy(l, arr + start, sizeof(int) * len_l);
    memcpy(r, arr + mid + 1, sizeof(int) * len_r);

    int i = 0, j = 0, k = start;
    while(i < len_l && j < len_r)
    {
        arr[k++] = l[i] < r[j] ? l[i++] : r[j++];
    }

    while(i < len_l)
    {
        arr[k++] = l[i++];
    }
}


/*
  函数功能：归并排序
  函数参数说明：
  arr: 待排序的数组
  start: 待排序数组的开始下标
  end: 待排序数组的结束下标
*/
void Merge_Sort(int* arr, int start, int end)
{
    if(start < end)
    {
        int mid = (start + end) / 2;
        //归
        Merge_Sort(arr, start, mid);
        Merge_Sort(arr, mid + 1, end);
        //并
        Merge(arr, start, mid, end);
    }
}

 
int main(int argc,char *argv[])
{
    int comm_sz;
    int my_rank;
    int length = 20; //数组大小，可修改
    int size; //每个进程所需数组的大小
    int* sub; //子数组，用于各进程内部的归并排序
    int* result = NULL; //结果数组，存储最终结果
    int i;
    int arr[length]; //待排序的数组

    //MPI初始化
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //在分发(Scatter)数据前，若进程号为0(根进程)，则对数组进行随机数初始化并打印初始数组信息
    if(my_rank == 0){
        printf("初始数组为:\n");
        for(i = 0;i <= length-1; i++){
            arr[i] = rand()%50; //每个元素的值控制在0-49
            printf("%d ",arr[i]);
        }
        printf("\n");
    }

    size = length / comm_sz; //计算每个进程所需数组的大小
    sub = (int*)malloc(size * sizeof(int)); //为进程所用数组开辟空间
 
    MPI_Scatter(arr, size, MPI_INT, sub, size, MPI_INT, 0, MPI_COMM_WORLD); //将数组元素分发给每个进程
 
    Merge_Sort(sub, 0, size-1); //将每个进程分到的子数组进行归并排序
 
    //在分发(Scatter)数据后，若进程号为0(根进程)，则开辟结果所需的数组空间（与输入数组一样大）
    if(my_rank == 0) {
        result = (int *)malloc(length * sizeof(int));  
    }
 
    MPI_Gather(sub, size, MPI_INT, result, size, MPI_INT, 0, MPI_COMM_WORLD); //将各进程排序的数组收集到0号进程(根进程)

    //在收集(Gather)各进程数据到根进程后，若进程号为0(根进程)，将收集的结果进行最后一次归并排序
    if (my_rank == 0) {
        Merge_Sort(result, 0, length-1);
        //打印排序后的结果
        printf("\n排序后的结果为:\n");
        for (i = 0;i <= length-1; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);//释放内存
    }
    free(sub);//释放内存
    
    MPI_Finalize();//终止MPI模拟环境
    return 0;
}
