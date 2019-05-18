/* CS16BTECH11016 - PCP Programming Assignment-II*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <random>
#include <chrono>
#include <bits/stdc++.h>
#include <semaphore.h>
#include <random>
#include <time.h>
#include <limits.h>
using namespace std;



class Bakery
{
public:
	bool* flag;
	long long int * label;
	int n;
	Bakery(int n)
	{
		flag = new bool[n];
		label = new long long int[n];
		for(int i=0;i<n;i++)
		{
			flag[i] = false;
			label[i] = 0;
		}
	}
	Bakery()
	{
		delete flag;
		delete label;
	}
	void Lock(int thread_id)
	{
		
		
		    flag[thread_id] = true;
		    int max=0;
		    for(int j=0;j<n;j++)
		    {
		    	if(label[j]>max)	max = label[j];
		    }
		    if(label[thread_id] > 2847483647)	
		    {
		    	exit(1);
		    }
		    label[thread_id] = max+1;
	
   			for(int k=0;k<n;k++)
   			{
      			while((k!=thread_id)&&(flag[k] &&((label[k]<label[thread_id])&&(k<thread_id)))) { }
    		}
  		
	}

	void Unlock(int thread_id)
	{
		flag[thread_id] = false;
	}
};

int n,l1,l2;
int k;
double *avg,tot_waiting_time=0;
FILE *f;


const std::string currentDateTime() 
{                                            //function to print the system time
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}

string timeconv(time_t tc)
{
	string te = currentDateTime();
	return te;
}

Bakery *hello;

void testCS(int id)					// test CS function into which all threads are sent
{
  time_t invtime,enttime,extime,cstime,remtime,reqtime;
 
  time_t start,end;

  double const exp_dist_lambda_1 = l1;
  double const exp_dist_lambda_2 = l2;

  int seed = chrono::system_clock::now().time_since_epoch().count();

  default_random_engine rg(seed);
  exponential_distribution <double> dist_1(exp_dist_lambda_1);
  exponential_distribution <double> dist_2(exp_dist_lambda_2);   // exponential distribution over l1 and l2

  double t1d = dist_1(rg) + 0.5;
  double t2d = dist_2(rg) + 0.5;

  int t1 = (int)t1d;
  int t2 = (int)t2d;  	  

  for(int i=0;i<k;i++)
  {

  	time(&reqtime);					// request to enter the CS
  	start = clock();
  	fprintf(f," %d CS request by %d thread at time %s\n",i,id,timeconv(reqtime).c_str());

    hello->Lock(id);              // lock mechanism

    time(&enttime);
    end = clock();
    tot_waiting_time = tot_waiting_time+ (double)(end-start)/CLOCKS_PER_SEC;
    //cout<<tot_waiting_time<<endl;
    fprintf(f," %d CS enter by %d thread at time %s\n",i,id,timeconv(enttime).c_str());		// entering the CS

    sleep(t1);		//sleep for some time

    hello->Unlock(id); 
    
  	time(&invtime);
  	fprintf(f," %d CS exit by %d thread at time %s\n",i,id,timeconv(invtime).c_str());
    sleep(t2);
  }
}

int main()
{
	ifstream fp;
	fp.open("inp-params.txt");//taking input from file
    fp>>n>>k>>l1>>l2;
  	hello = new Bakery(n);
	f = fopen("Bakery-log.txt", "w");
  	thread t1[n];
 	for (int i = 0; i < n; ++i)
	{
   	t1[i] = thread(testCS,i);

	}
	for (int i = 0; i < n; ++i)
  {
 	 	t1[i].join();
	}
	double average_waiting_time = (double)tot_waiting_time/(n*k) ;
	cout<<"average waiting time is :"<<average_waiting_time<<endl;
}
