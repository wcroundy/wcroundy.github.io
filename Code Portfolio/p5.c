//This program takes a text document as an input.  the text docuament would contain a list of integers 
//representing a process arrival time follow by the process burst time.  The program then processes through
//various algorithms for cpu scheduling: first come first served (fcfs), shortest job first (sjf), and 
//shortest time remaining first (strf).  Each alfgorithm displays average response time, wait time, and turnaround 
//time for comparison purposes.


#include<stdio.h>
#include<math.h>
#include<stdlib.h>



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

    printf("First Come First Served\n");
    printf("Avg. Resp.: %f, Avg. Wait.: %f, Avg. T.A.: %f\n\n",wait, wait, turn);
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
    printf("First Come First Served\n");
    printf("Avg. Resp.: %f, Avg. Wait.: %f, Avg. T.A.: %f\n\n",response, response, turnaround);
}


//shortest time remaining first
int strf(int started[], int duration[], int size)
{

    printf("Shortest Time Remaining First\n");
	double ta = 0;
	double w = 0;
	double res = 0;
	int available[25], dur[25], start[25], done[25], wait[25];
	int clk = started[0];
	int avcount = 0;
	int donecount = 0;
	int i;
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
            printf("Duration less than zero, exit\n\n");
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

    printf("First Come First Served\n");
    printf("Avg. Resp.: %f, Avg. Wait.: %f, Avg. T.A.: %f\n\n",res, w, ta);

	return 0;
}



int main(int argc, char * argv[])
{
    int num;
    int start[100];
    int duration[100];
    int startcount = 0;
    int durcount = 0;
    int count = 0;
    while(fscanf(stdin, "%d", &num) == 1)
    {
        if(count % 2 == 0)
        {
            start[startcount] = num;
            startcount++;
        }
        else
        {
            duration[durcount] = num;
            durcount++;
        }
        count++;
    }
    count = count / 2;
    printf("Wade Colby Roundy\n Assignment 5: CPU Scheduling\n\n");
    fcfs(start, duration, count);
	sjf(start, duration, count);
	strf(start, duration, count);
    return 0;
}