//The program processes through
//various algorithms for cpu scheduling: first come first served (fcfs), shortest job first (sjf), and 
//shortest time remaining first (strf).  Each alfgorithm displays average response time, wait time, and turnaround 
//time for comparison purposes.  The main function simply provides test arrays to represent process arrival time and
//processes burst times.

#include<iostream>
#include<math.h>
#include<stdlib.h>
#include <cstdlib>
using namespace std;


void fcfs(int start[], int duration[], int size)
{
	double wait = 0;
	double turn = 0;
	int started[25], done[25];
	int clk = start[0];
	int count = 0;
	while (count < size)
	{
		started[count] = clk;
		clk = clk + duration[count];
		done[count] = clk;
		count++;
	}
	int i;
	for (i = 0; i < size; i++)
	{
		wait = wait + started[i] - start[i];
		turn = turn + done[i] - start[i];
	}
	wait = wait / size;
	turn = turn / size;
	cout << "First Come First Served" << endl;
	cout << "Avg. Resp.: " << wait << ",  " << "Avg. Wait.: " << wait << ",  " << "Avg. T.A.: " << turn << endl;
	cout << endl;
}



void sjf(int started[], int duration[], int size)
{
	int begin[25], done[25], available[25], start[25];
	int i;
	double response = 0;
	double turnaround = 0;
	int index = 0;
	int complete = 0;
	int clk = started[0];
	//fill available with -1 for empty, fill start with values
	for (i = 0; i < 25; i++)
	{
		available[i] = -1;
		start[i] = started[i];
		if (i > size)
		{
			start[i] = -1;
		}
	}
	while (complete < size)
	{
		//holds index of shortest available process
		int temp = 99999;
		//determine available processes
		for (i = 0; i < size; i++)
		{
			//determine available processes not already processed
			if (start[i] <= clk && start[i] >= 0)
			{
				available[i] = start[i];
			}
			else
			{
				available[i] = -1;
			}
		}
		//determine shortest available process
		for (i = 0; i < size; i++)
		{
			if (duration[i] < temp && available[i] >= 0)
			{
				temp = available[i];
				index = i;
			}
		}
		//index now holds the index of the shortest available process
		//proceed with shortest available process, increment clock
		begin[index] = clk;
		clk = clk + duration[index];
		done[index] = clk;

		//assure no repeats
		start[index] = -1;
		complete++;
	}
	//calculate values
	//totals
	for (i = 0; i < size; i++)
	{
		response = response + begin[i] - started[i];
		turnaround = turnaround + done[i] - started[i];
	}
	//averages
	response = response / size;
	turnaround = turnaround / size;
	//print calculated values
	cout << "Shortest Job First" << endl;
	cout << "Avg. Resp.: " << response << "   " << "Avg. Wait.: " << response << "   " << "Avg. T.A.: " << turnaround << endl;
	cout << endl;
}


//shortest time remaining first
int strf(int started[], int duration[], int size)
{
	cout << "Shortest Time Remaining First" << endl;
	double ta = 0;
	double w = 0;
	double res = 0;
	int available[25], dur[25], start[25], done[25], wait[25];
	int clk = started[0];
	int avcount = 0;
	int donecount = 0;
	int i;
	int oclk = 0;
	int high = -1;
	int index = 0;
	//initialize array values
	for (i = 0; i < size; i++)
	{
		available[i] = -1;
		start[i] = -1;
		dur[i] = duration[i];
		wait[i] = 0;
	}
	//repeat until all processes are run (dur = 0)
	while (donecount < size)
	{

		//determine available processes
		for (i = 0; i < size; i++)
		{
			if (started[i] <= clk && dur[i] > 0)
			{
				available[i] = i;
				avcount++;
				high = i;
			}
			else
			{
				available[i] = -1;
			}
		}

		//determine shortest available process
		int temp = 99999;
		for (i = 0; i < avcount; i++)
		{
			if (available[i] >= 0 && dur[i] < temp)
			{
				temp = dur[i];
				index = i;
			}
		}

		//determine increment
		int inc;
		//if we are on the last burst we don't compare to the next process
		if (index == size - 1 || donecount == size - 1 || high + 1 == size)
		{
			inc = dur[index];

		}
		//if index process will finishe before a new process arrives
		else if (dur[index] + clk < started[high + 1])
		{
			inc = dur[index];
		}
		//if a new process arrives before index process finishes
		else
		{
			inc = started[index + 1] - clk;
		}

		//process shortest available process by increment
		//set start time if not already
		if (start[index] < 0)
		{
			start[index] = clk;
		}
		//increment clk
		clk = clk + inc;
		//decrement burst time of selected process
		dur[index] = dur[index] - inc;
		//set done time if process if finished
		if (dur[index] == 0)
		{
			done[index] = clk;
			donecount++;
		}
		// error if we get a bad increment, this means that the increment was not right and we went below zero on the duration
		else if (dur[index] < 0)
		{
			cout << "duration less than zero" << endl;
			cout << endl;
			return -1;
		}
		//calculate wait times
		for (i = 0; i < size; i++)
		{
			if (available[i] >= 0 && i != index)
			{
				wait[i] = wait[i] + inc;
			}
		}
	}
	//calculate totals and print arrays
	for (i = 0; i < size; i++)
	{
		w = w + wait[i];
		res = res + start[i] - started[i];
		ta = ta + done[i] - started[i];
	}
	//calculate and print averages
	ta = ta / 4;
	w = w / 4;
	res = res / 4;
	cout << "Avg. Resp.: " << res << ",  " << "Avg. Wait.: " << w << ",  " << "Avg. T.A.: " << ta << endl;
	cout << endl;
	return 0;
}


int main()
{
	int count = 4;
	int start[4] = { 5,8,15,17 };
	int duration[4] = { 8,10,3, 2 };
	fcfs(start, duration, count);
	sjf(start, duration, count);
	strf(start, duration, count);
	//rr2(start, duration, count);

	system("pause");
	return 0;
}