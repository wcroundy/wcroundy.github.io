#include<iostream>
#include<math.h>
#include<stdlib.h>
#include <cstdlib>
using namespace std;



void fcfs(int inputs[], int size)
{
	int total = 0;
	int i;
	for (i = 0; i < size - 1; i++)
	{
		total = total + abs(inputs[i] - inputs[i + 1]);
	}
	cout << "FCFS Total Seek: " << total << endl;
}


void sstf(int input[], int size)
{
	int done[100];
	int total = 0;
	int donecount = 1;
	int dif = 0;
	int start = input[0];
	int index = 0;
	int i;
	//initialize done array to be less than zero
	for (i = 0; i < size; i++)
	{
		done[i] = -1;
	}
	done[0] = 1;
	//loop until all are done
	while (donecount < size)
	{
		int temp = 99999;
		//determine shortest jump
		for (i = 0; i < size; i++)
		{
			//only compare is we haven't jumped to this point yet
			if (done[i] < 0)
			{
				dif = abs(start - input[i]);
				//if the difference is less than the previous difference, this will always be true on the first loop
				if (dif < temp)
				{
					temp = dif;
					index = i;
				}
			}
		}
		//set the new start value
		start = input[index];
		//add to the total
		total = total + temp;
		//set done for the used index to 1
		done[index] = 1;
		//increment donecount
		donecount++;
	}
	//print total
	cout << "SSTF Total Seek: " << total << endl;
}


void look(int input[], int size)
{
	int done[100];
	int total = 0;
	int donecount = 1;
	int dif = 0;
	int start = input[0];
	int current = start;
	int index = 0;
	int i;
	//initialize done array to be less than zero
	for (i = 0; i < size; i++)
	{
		done[i] = -1;
	}
	done[0] = 1;
	//loop until all are done
	while (donecount < size * 2)
	{
		int temp = 99999;
		if (donecount < size)
		{
			for (i = 0; i < size; i++)
			{
				//if the value is greater than current and hasn't been done
				if (input[i] >= current && done[i] < 0)
				{
					dif = abs(current- input[i]);
					//if the difference is less than the previous difference, this will always be true on the first loop
					if (dif < temp)
					{
						temp = dif;
						index = i;
					}
				}

			}
		}
		else if (donecount >= size)
		{
			for (i = 0; i < size; i++)
			{
				//if the value is less than current and hasn't been done
				if (input[i] <= current && done[i] < 0)
				{
					dif = abs(current - input[i]);
					//if the difference is less than the previous difference, this will always be true on the first loop
					if (dif < temp)
					{
						temp = dif;
						index = i;
					}
				}

			}
		}
		if (temp < 99999)
		{
			total = total + temp;
			done[index] = 1;
			current = input[index];
		}
		donecount++;
	}
	cout << "LOOK Total Seek: " << total << endl;
}



void clook(int input[], int size)
{
	int done[100];
	int total = 0;
	int donecount = 1;
	int dif = 0;
	int start = input[0];
	int current = start;
	int index = 0;
	int i;
	//initialize done array to be less than zero
	for (i = 0; i < size; i++)
	{
		done[i] = -1;
	}
	done[0] = 1;
	//loop until all are done
	while (donecount < size * 2)
	{
		int temp = 99999;
		if (donecount < size)
		{
			for (i = 0; i < size; i++)
			{
				//if the value is greater than current and hasn't been done
				if (input[i] >= current && done[i] < 0)
				{
					dif = abs(current - input[i]);
					//if the difference is less than the previous difference, this will always be true on the first loop
					if (dif < temp)
					{
						temp = dif;
						index = i;
					}
				}


			}
		}
		else if (donecount == size)
		{
			for (i = 0; i < size; i++)
			{
				if (input[i] < start)
				{
					start = input[i];
				}
			}
			total = total + abs(current - start);
			current = start;
		}
		if (donecount >= size)
		{
			for (i = 0; i < size; i++)
			{
				//if the value is less than current and hasn't been done
				if (input[i] >= current && done[i] < 0)
				{
					dif = abs(current - input[i]);
					//if the difference is less than the previous difference, this will always be true on the first loop
					if (dif < temp)
					{
						temp = dif;
						index = i;
					}
				}

			}
		}
		if (temp < 99999)
		{
			total = total + temp;
			done[index] = 1;
			current = input[index];
		}
		donecount++;
	}
	cout << "CLOOK Total Seek: " << total << endl;
}


int main(int argc, char * argv[])
{
	int inputs[20] = { 221, 16, 103, 101, 4, 99, 84, 23, 72, 245, 231, 61, 247, 233, 212, 85, 193, 115, 29, 35 };
	int size = 20;
	fcfs(inputs, size);
	sstf(inputs, size);
	look(inputs, size);
	clook(inputs, size);
	system("pause");
	return 0;
}