//All Kinds of sorting
#include <iostream>
#include <vector>
#include<math.h>
#include <limits.h>
#include <queue>
#include <functional>
const int MAXNUM = 100000;
//
struct MaxHeap{
	int *value;
	int HeapSize;
	int HeapCap;
};
//
using namespace std;
void BubbleSort(int *a, int n);
void InsertSort(int *a, int n);
void ShellSort(int *a, int n);
void PrintOut(int *a, int n);
void SelectSort(int *a, int n);
void HeapSort1(int *a, int n);
//
void HeapSort2(int *a, int n);
MaxHeap * CreateMaxHeap();
void ManageHeap(MaxHeap *m, int node);
//
void MergeSort1(int *a, int n);
void MSort(int *a, int start, int end, int *tmp);
void Merge(int *a, int start, int right, int end,int *tmp);
//
void MergeSort2(int *a, int n);
void MSort2(int *a, int n, int length, int *tmp);
void Merge2(int *a, int start, int right, int end,int *tmp);
//
void QuickSort(int *a, int n);
void QSort(int *a, int start, int end);
int Getpiot(int *a, int start, int end);
int main(){
	int n;
	cin>>n;
	int *input = new int[MAXNUM];
	for (int i = 0; i < n; i ++)
	{
		cin>>input[i];
	}
	MergeSort2(input,n);
	PrintOut(input,n);
	return 0;
}
void BubbleSort(int *a, int n){
	for (int i = n - 1; i >= 0 ; i--)//bubble to the bottom end
	{
		bool flag = 0;
		for (int j = 0; j < i; j++)
		{
			if (a[j] > a[j+1])//wrong location
			{
				//swapping
				int temp = a[j];
				a[j] = a[j+1];
				a[j+1] = temp;
				flag = 1;
			}
		}
		if (flag == 0)  break;//The orders are now in sequence
	}	
}
void InsertSort(int *a, int n){
	for (int i = 1 ; i < n; i = i ++)
	{
		//insert a number
		int value = a[i], j;		
		for (j = i; j >= 1 && a[j-1] > value; j = j - 1)
		{
			a[j] = a[j-1];//pass the number
		}
		a[j] = value;//now we know the location the value should be inserted
	}	
}
void PrintOut(int *a, int n){
	for (int i = 0; i < n;i++)
	{
		cout<<a[i];
		if (i!=n-1)
		{
			cout<<" ";
		}
	}
}
void ShellSort(int *a, int n){
	int times = log(n)/log(2);
	for (; times>0;times--)
	{
		int seq = pow(2,times) - 1;		//using Hibbard sequence
		for (int i = seq ; i < n; i = i ++)
		{
			//insert a number
			int value = a[i], j;		
			for (j = i; j >= seq && a[j-seq] > value; j = j - seq)
			{
				a[j] = a[j-seq];
			}
			a[j] = value;//now we know the location the value should be inserted
		}
	}
}
void SelectSort(int *a, int n){
	for (int i = 0; i < n ; i++)
	{
		int min_loc = i;
		for (int j = i+1; j < n; j++)
		{
			if (a[j] < a[min_loc])
			{
				min_loc = j; 
			}
		}
		//swapping
		int temp = a[i];
		a[i] = a[min_loc];
		a[min_loc] = temp;
	}
}
void HeapSort1(int *a, int n){
	priority_queue<int,vector<int>,greater<int>> q;//minheap
	for (int i = 0; i < n; i++)
	{
		q.push(a[i]);
	}
	for (int i = 0 ; i < n; i++)
	{
		a[i] = q.top();
		q.pop();
	}
}
void HeapSort2(int *a, int n){
	MaxHeap *M = CreateMaxHeap();
	M->value = a;
	M->HeapSize = n;
	//build a maxheap first(crucial)
	for (int i = (n-1)/2; i >= 0; i--)
	{
		ManageHeap(M,i);
	}
	for (int i = n - 1 ; i>= 1; i--)
	{
		//swapping
		int temp = a[0];
		a[0] = a[i];
		a[i] = temp;
		M->HeapSize--;
		ManageHeap(M,0);
	}
}
MaxHeap * CreateMaxHeap(){
	MaxHeap *M = new MaxHeap;	
	M->HeapSize = 0;
	M->HeapCap = MAXNUM + 1;
	return M;
}
void ManageHeap(MaxHeap *m, int i){
	int num = m->HeapSize;		
	if (i > (num-1)/2)
	{
		return;//leaf node, no need to manage
	}	
	int value = m->value[i];
	//from the node j, build the min heap from behind
	while (1)
	{
		int left_child = i*2+1, right_child = i*2+2;
		bool flag = 1;
		int lvalue, rvalue;
		if (left_child >= num)//violating the boundary
		{
			lvalue = INT_MIN;
		}
		else lvalue = m->value[left_child];
		if (right_child >=num)
		{
			rvalue = INT_MIN;
		}
		else
			rvalue = m->value[right_child];
		if (lvalue >= rvalue)
		{
			if (lvalue > value)
			{
				m->value[i] = m->value[left_child];
				i = left_child;
				flag = 0;
			}
		}
		else
		{
			if (rvalue > value)
			{
				m->value[i] = m->value[right_child];
				i = right_child;
				flag = 0;
			}
		}
		if (flag == 1)
		{
			break;
		}
	}
	m->value[i] = value;	
}
//
void MergeSort1(int *a, int n){
	int *tmp = new int[MAXNUM];
	MSort(a,0,n-1,tmp);
	delete[] tmp;
}
void MSort(int *a, int start, int end, int *tmp){
	int center = (start+end)/2;
	if (start < end)
	{
		MSort(a,start,center,tmp);
		MSort(a,center+1,end,tmp);
		Merge(a,start,center+1,end,tmp);
	}
}
void Merge(int *a, int start, int right, int end,int *tmp){
	int i = start, j = right, num = 0;
	while (i <= right - 1 && j <=end)
	{
		if (a[i] < a[j])
		{
			tmp[num++] = a[i++];
		}
		else{
			tmp[num++] = a[j++];
		}
	}
	while (i <= right - 1)  
		tmp[num++] = a[i++];  
	while (j <= end)  
		tmp[num++] = a[j++];  
	for (i = 0; i < num; i++)  
		a[start+i] = tmp[i];  
}
void MergeSort2(int *a, int n){
	int *tmp = new int[MAXNUM];
	int length = 1;
	while(length < n){
		MSort2(a,n,length,tmp);
		length = length*2;
		MSort2(tmp,n,length,a);
		length = length *2;
	}
	delete[] tmp;
}
void MSort2(int *a, int n, int length, int *tmp){
	int i = 0;
	for (; i <= n - 2*length; i = i + 2*length)
	{
		Merge2(a,i,i+length,i+2*length-1,tmp);
	}
	//deal with the endding side of the data
	//there are two sub_lists
	if (i+length < n)
	{
		Merge2(a,i,i+length,n-1,tmp);
	}
	else{//only one sub list
		for (; i < n;i++)
		{
			tmp[i] = a[i];
		}
	}
}
void Merge2(int *a, int start, int right, int end,int *tmp){
	int i = start, j = right, num = start;
	while (i <= right - 1 && j <=end)
	{
		if (a[i] < a[j])
		{
			tmp[num++] = a[i++];
		}
		else{
			tmp[num++] = a[j++];
		}
	}
	while (i <= right - 1)  
		tmp[num++] = a[i++];  
	while (j <= end)  
		tmp[num++] = a[j++]; 
}
void QuickSort(int *a, int n){
	QSort(a,0,n-1);
}
void QSort(int *a, int start, int end){ 
	int CutNum = 50;
	int length = end-start+1;
	if(length < CutNum){
//�ԱȽ�С������ֱ���ò����������
		InsertSort(a+start,length);
		return ;
	}
	int piot = Getpiot(a,start,end);//Ѱ����Ԫ
	int i = start+1, j = end - 2;//start����Ԫ�ؿ϶�С����Ԫ��end����Ԫ�ؿ϶�������Ԫ����Ԫ��end-1����������������д��
	while(1){
		while (a[i] < piot)//�ҵ���һ��Υ�������ͷ������
		{
			i++;
		}
		while (a[j] > piot)//�ҵ���һ��Υ�������β������
		{
			j--;
		}
		if (i < j)//����λ�á����ð�����Υ����������ֽ�����������Ͳ�Υ����
		{
			int temp = a[i];
			a[i] = a[j];
			a[j] = temp;
			i++;//������ָ��λ�û��ü����䣡����һ��û�����Թ����ˣ���Ϊ�������ֺ���Ԫ��ͬ��ʱ��Ῠ����
			j--;
		}
		else//�Ѿ��ߵ������ˣ�Ӧ���˳�
			break;
	}
	//������Ԫ��a[i]��λ��
	int temp = a[i];
	a[i] = a[end-1];
	a[end-1] = temp;
	//��Ԫ�Ѿ��ź�λ���ˣ��������λ����ȷ���ġ�����ݹ�Ķ���Ԫǰ��ͺ�������鲿�ֽ��������õݹ�ķ�������֮
	QSort(a,start,i-1);
	QSort(a,i+1,end);
}
int Getpiot(int *a, int start, int end){
	int medium = (start+end)/2, temp;
	//���ڵ�������Ԫ�ص�����£�������medium�Ŀ�����
	//���һ��Ҫa[start]<a[medium]<a[end]
	if (a[start] > a[medium])
	{
		temp = a[start];
		a[start] = a[medium];
		a[medium] = temp;
	}
	if (a[start] > a[end])//��ʱ�Ѿ�������������С��Ԫ�ص�����a[start]��
	{
		temp = a[start];
		a[start] = a[end];
		a[end] = temp;
	}
	if (a[medium] > a[end])//��ʱ�Ѿ���������������Ԫ�ص�����a[end]��
	{
		temp = a[medium];
		a[medium] = a[end];
		a[end] = temp;
	}
	//����Ԫ��Ԫ�ص�����a[end-1]��
	temp = a[end - 1];
	a[end-1] = a[medium];
	a[medium] = temp;
	return a[end-1];
}