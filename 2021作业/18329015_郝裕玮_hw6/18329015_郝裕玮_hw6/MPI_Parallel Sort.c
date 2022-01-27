#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

//Merge������Merge_Sort�������ڹ鲢����
 
/*
  �������ܣ��ϲ�
  ��������˵����
  arr: Ŀ������
  start: ���ϲ��εĿ�ʼ�±�
  mid: ���ϲ��ε��м��±�
  end: ���ϲ��εĽ����±�
*/
void Merge(int* arr, int start, int mid, int end)
{
    int len_l, len_r; //���Ҵ��ϲ�����ĳ���
    len_l = mid - start + 1;
    len_r = end - mid;
    int l[len_l], r[len_r]; //������ʱ���飬�ֱ𱣴���ϲ�����������
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
  �������ܣ��鲢����
  ��������˵����
  arr: �����������
  start: ����������Ŀ�ʼ�±�
  end: ����������Ľ����±�
*/
void Merge_Sort(int* arr, int start, int end)
{
    if(start < end)
    {
        int mid = (start + end) / 2;
        //��
        Merge_Sort(arr, start, mid);
        Merge_Sort(arr, mid + 1, end);
        //��
        Merge(arr, start, mid, end);
    }
}

 
int main(int argc,char *argv[])
{
    int comm_sz;
    int my_rank;
    int length = 20; //�����С�����޸�
    int size; //ÿ��������������Ĵ�С
    int* sub; //�����飬���ڸ������ڲ��Ĺ鲢����
    int* result = NULL; //������飬�洢���ս��
    int i;
    int arr[length]; //�����������

    //MPI��ʼ��
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //�ڷַ�(Scatter)����ǰ�������̺�Ϊ0(������)�������������������ʼ������ӡ��ʼ������Ϣ
    if(my_rank == 0){
        printf("��ʼ����Ϊ:\n");
        for(i = 0;i <= length-1; i++){
            arr[i] = rand()%50; //ÿ��Ԫ�ص�ֵ������0-49
            printf("%d ",arr[i]);
        }
        printf("\n");
    }

    size = length / comm_sz; //����ÿ��������������Ĵ�С
    sub = (int*)malloc(size * sizeof(int)); //Ϊ�����������鿪�ٿռ�
 
    MPI_Scatter(arr, size, MPI_INT, sub, size, MPI_INT, 0, MPI_COMM_WORLD); //������Ԫ�طַ���ÿ������
 
    Merge_Sort(sub, 0, size-1); //��ÿ�����̷ֵ�����������й鲢����
 
    //�ڷַ�(Scatter)���ݺ������̺�Ϊ0(������)���򿪱ٽ�����������ռ䣨����������һ����
    if(my_rank == 0) {
        result = (int *)malloc(length * sizeof(int));  
    }
 
    MPI_Gather(sub, size, MPI_INT, result, size, MPI_INT, 0, MPI_COMM_WORLD); //������������������ռ���0�Ž���(������)

    //���ռ�(Gather)���������ݵ������̺������̺�Ϊ0(������)�����ռ��Ľ���������һ�ι鲢����
    if (my_rank == 0) {
        Merge_Sort(result, 0, length-1);
        //��ӡ�����Ľ��
        printf("\n�����Ľ��Ϊ:\n");
        for (i = 0;i <= length-1; i++) {
            printf("%d ", result[i]);
        }
        printf("\n");
        free(result);//�ͷ��ڴ�
    }
    free(sub);//�ͷ��ڴ�
    
    MPI_Finalize();//��ֹMPIģ�⻷��
    return 0;
}
