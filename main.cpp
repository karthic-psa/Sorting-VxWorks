#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <complex>
#include "taskHookLib.h"
#include "taskLib.h"
#include "sysLib.h"
#include "text.h"
#include "vxWorks.h"
#include "tickLib.h"
#define INCLUDE_TASK_HOOKS
using namespace std;
#define PRIORITYL 255
#define PRIORITYM 128
#define PRIORITYH 0

void tstart(int a);
void tdone(int a);
void tswitch(int last, int next);
char removesplchars(char a[]);
int removes(char* s);
void randomize_priority();
void priorityget();

void insert_sort_intialize();
void insert_sort(char arr[], int length);
void select_sort_intialize();
void select_sort(char arr[], int n);
void heap_sort_initialize();
void maxheap (char* a, int i, int n);
void heap_sort(char* a,int n);
void maxbuildheap(char* a,int n);
void change (vector<string> &data, int i, int j);
void out(vector<string>);
void task_info_put();

vector<string> erase();
vector<string> div();
void initial();

int sizeofstring;
int no_of_preempts = 3;
int hscounter;

int ispreempts=0,sspreempts=0,hspreempts=0;
struct TaskInfo
{
int priority;
bool current;
int taskid;
int s_ticks;
int end_ticks;
int exe_ticks;
}is,ss,hs;

double cctime;
double note_ticks;
vector<string> forsort;

int main()
{
initial();
note_ticks = 0;
cctime = sysClkRateGet();
cctime = 1/cctime;
removes(sourceString);
sizeofstring = sizeof(sourceString);
cout << "String removed of non-alphabetic characters." << endl;
tickSet(0);
taskCreateHookAdd((FUNCPTR)tstart);
taskSwitchHookAdd((FUNCPTR)tswitch);
taskDeleteHookAdd((FUNCPTR)tdone);
is.taskid = taskSpawn("Insert Sort", is.priority, 0, 1000000, (FUNCPTR)insert_sort_intialize, 0 ,0,0,0,0,0,0,0,0,0);
ss.taskid = taskSpawn("Select Sort", ss.priority, 0, 1000000, (FUNCPTR)select_sort_intialize, 0,0,0,0,0,0,0,0,0,0);
hs.taskid = taskSpawn("Heap Sort", hs.priority, 0, 1000000, (FUNCPTR)heap_sort_initialize, 0,0,0,0,0,0,0,0,0,0);
cout<< "wait 150 ticks for tasks to end" <<endl;
taskDelay(150);
taskCreateHookDelete((FUNCPTR)tstart);
taskSwitchHookDelete((FUNCPTR)tswitch);
taskDeleteHookDelete((FUNCPTR)tdone);
task_info_put();
return 0;
}

void initial()
{
is.s_ticks=0;
is.end_ticks=0;
is.exe_ticks=0;
is.current=false;
is.priority=PRIORITYH;
ss.s_ticks=0;
ss.end_ticks=0;
ss.exe_ticks=0;
ss.current=false;
ss.priority=PRIORITYM;
hs.s_ticks=0;
hs.end_ticks=0;
hs.exe_ticks=0;
hs.current=false;
hs.priority=PRIORITYL;
}


void task_info_put()
{	cout<<endl;
	cout<<"\n The clock Cycle time is: "<<cctime<<endl;
	cout<<endl<<"Insert Sort"<<endl;
	cout<<"The Insert Sort Start Ticks is: "<<is.s_ticks<<endl;
	cout<<"The Insert Sort End Ticks is: "<<is.end_ticks<<endl;
	cout<<"The Insert Sort End Time is: "<<(double)is.end_ticks*cctime<<endl;
	cout<<"The Insert Sort Exe Ticks is: "<<is.exe_ticks<<endl;
	cout<<"The Insert Sort Exe Time is: "<<(double)is.exe_ticks*cctime<<endl;
	cout<<"The Insert Sort Preemptions are: "<<ispreempts<<endl;
	cout<<endl;
	cout << "Select Sort" <<endl;
	cout << "The Select Sort Start Time is: " << ss.s_ticks << endl;
	cout << "The Select Sort End Ticks is: " << ss.end_ticks<< endl;
	cout << "The Select Sort End Timeis: " << (double)ss.end_ticks*cctime << endl;
	cout << "The Select Sort Execution Ticks is: " << ss.exe_ticks<< endl;
	cout << "The Select Sort Execution Time is:" << (double)ss.exe_ticks*cctime << endl;
	cout<<  "The Select Sort Preemptions are: "<<sspreempts<<endl;
	cout <<  endl;
	cout << "Heap Sort" <<endl;
	cout << "The Heap Sort Start Time is: " << hs.s_ticks<< endl;
	cout << "The Heap Sort End Ticks is: " << hs.end_ticks << endl;
	cout << "The Heap Sort End Time is: " << (double)hs.end_ticks * cctime << endl;
	cout << "The Heap Sort Execution Ticks is: " << hs.exe_ticks<< endl;
	cout << "The Heap Sort Execution Time is:" <<(double) hs.exe_ticks * cctime << endl;
	cout<<  "The Heap Sort Preemptions are:" <<hspreempts<<endl;
	cout << endl;
	cout<<"Done. "<<endl;
}

void tstart(int a)
{
	int stime = tickGet();
	string task = taskName(taskIdSelf());
	if (task == "Insert Sort")
	{
		is.s_ticks = stime;
		is.current=true;
	}
	if (task == "Select Sort")
	{
		ss.s_ticks = stime;
		ss.current = true;
	}
	if (task == "Heap Sort")
	{
	hs.s_ticks = stime;
	hs.current=true;
	}
	}

void tswitch(int last, int next)
{
	int ctime=tickGet();
//	cout<<"ctime ="
	string lasttask=taskName(last);
	string nexttask=taskName(next);
	int lastswitchat = ctime-note_ticks;
	if(lasttask == "Insert Sort")
	{
		is.exe_ticks = is.exe_ticks+lastswitchat;
		ispreempts++;
	}
	if(lasttask == "Select Sort")
	{
		ss.exe_ticks = ss.exe_ticks+lastswitchat;
		sspreempts++;
			}
	if(lasttask == "Heap Sort")
	{
		hs.exe_ticks = hs.exe_ticks+lastswitchat;
		hspreempts++;
	}
	note_ticks=ctime;
}

void tdone(int a)
{
	int endtime = tickGet();
	string task = taskName(taskIdSelf());
	if (task == "Insert Sort")
	{
	is.end_ticks=endtime;
	is.current=false;
	}
	if (task == "Select Sort")
	{
	ss.end_ticks = endtime;
	ss.current = false;
	}
	if (task == "Heap Sort")
	{
	hs.end_ticks = endtime;
	hs.current = false;
}
}
void randomize_priority()
{
	int t;
	t=is.priority;
	is.priority=ss.priority;
	ss.priority=hs.priority;
	hs.priority=t;
	taskPrioritySet(hs.taskid, hs.priority);
	taskPrioritySet(is.taskid, is.priority);
	taskPrioritySet(ss.taskid, ss.priority);
}

int removes(char* s)
{
    char* cpy = s;  // an alias to iterate through s without moving s
    char* temp = s;
    while (*cpy)
    {
        if (*cpy != ' ')
            *temp++ = *cpy;
        cpy++;
    }
    *temp = 0;

    cout << s << endl;
    cout<<"\n";
    int h=0;
    h=strlen(s);
    cout<<h<<endl;
    cout<<"\n";
    return h;
}

void insert_sort_intialize()
{	int a=0;
	removesplchars(sourceString);
	a=removes(sourceString);
	cout<<endl<<"Insert Sort Result"<<endl;
	insert_sort(sourceString,a);
}

void insert_sort(char arr[], int length)
{
 	int j, temp;
 	int counts = 0;
 	int size=sizeofstring-1;
 	int preempts=(size/no_of_preempts)-1;
for (int i = 0; i < length; i++){
	j = i;

	while (j > 0 && arr[j] < arr[j-1]){
		  temp = arr[j];
		  arr[j] = arr[j-1];
		  arr[j-1] = temp;
		  j--;
		  }

if(i%preempts==0 && i!=0)
{
	randomize_priority();
}
}
for(int v=0;v<=length;v++)
{
    cout<<arr[v];
}
cout<<"\n";
}
void select_sort_intialize()
{
		int b=0;
		removesplchars(sourceString);
		b=removes(sourceString);
		cout<<endl<<"Select Sort Result"<<endl;
		select_sort(sourceString,b);
}
void select_sort(char arr[], int n)
{
	int pos_min,temp;
		int count = 0;
		int size=sizeofstring-1;
		int preempts=size/no_of_preempts;

		for (int i=0; i < n-1; i++)
		{
		    pos_min = i;

			for (int j=i+1; j < n; j++)
			{

			if (arr[j] < arr[pos_min])
	                   pos_min=j;
		}

		      if (pos_min != i)
	            {
	                 temp = arr[i];
	                 arr[i] = arr[pos_min];
	                 arr[pos_min] = temp;
	            }

	    count++;
	    if(count%preempts==0)
	    {
	    	randomize_priority();
	    }
		}
      	for(int j=0; j<=n; j++)
	    {
	        cout<<arr[j];
	    }
	    cout<<"\n";
}
void heap_sort_initialize()
{
	int c=0;
	removesplchars(sourceString);
			c=removes(sourceString);
			cout<<endl<<"Heap Sort Result"<<endl;
			maxbuildheap(sourceString,c);
			heap_sort(sourceString,c);
}

void maxheap (char* a, int i, int n)
{
    int j, temp;
   // int counts = 0;
    temp = a[i];
    j = 2*i;
    while (j <= n)
    {
        if (j < n && a[j+1] > a[j])
            j = j+1;
        if (temp > a[j])
            break;
        else if (temp <= a[j])
        {
            a[j/2] = a[j];
            j = 2*j;
        }
    }
    a[j/2] = temp;
    return;
}
void heap_sort(char* a,int n)
{
    int i, temp;
    int counts = 0;
    int preempts=sizeofstring/no_of_preempts;
    for (i = n; i >= 2; i--)
    {
        temp = a[i];
        a[i] = a[1];
        a[1] = temp;
        maxheap(a, 1, i - 1);
   hscounter++;
   if(hscounter==5)
   {
	   randomize_priority();
   }
    
}
   for(int z=0;z<=n;z++)
{
    cout<<a[z];
}
    cout<<"\n";
}
void maxbuildheap(char* a,int n)
{
	int i;
	for(i=n/2;i>=1;i--)
	{
		maxheap(a,i,n);
	}
}
char removesplchars(char a[])
{
	for (int i = 0; i < sizeof(sourceString); i++)
	{
	if ((sourceString[i] < 65)|| ((sourceString[i] > 90) && (sourceString[i]<97)) || sourceString[i] > 122)
	    {


		sourceString[i] = ' ';
	    }
	}
	char cleaned[sizeof(sourceString)];
	for (int j = 0; j < sizeof(sourceString); j++)
	{
	    cleaned[j];
	return cleaned[j];}
}
