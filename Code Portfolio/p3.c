//This program uses the pthread C library to 
//print out the prime factorization of the integers
//from the command line using a different thread for 
//each input integer. 

#include<stdio.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

typedef struct nums
{
        int original;
        int input;
        int count;
        int factors[1000];
}nums;


void printnums(nums outputs)
{
        printf("%d: ",outputs.original);
        int i;
        for (i = 0; i < outputs.count; i++)
        {
                printf("%d ", outputs.factors[i]);
        }
        printf("\n");
}


void* findfactors(void* args)
{
        nums* outputs = (nums*)args;
        int* ptr = &outputs->count;
        outputs->count = 0;
        outputs->original = outputs->input;
        int i;
        outputs->factors[0] = outputs->input;
        while (outputs->input % 2 == 0)
        {
                outputs->factors[outputs->count] = 2;
                outputs->input = outputs->input / 2;
                outputs->count++;
        }
        for (i = 3; i <= sqrt(outputs->input); i = i + 2)
        {
                while (outputs->input % i == 0)
                {
                        outputs->factors[outputs->count] = i;
                        outputs->input = outputs->input / i;
                        outputs->count++;
                }
        }
        if (outputs->input > 2)
        {
                outputs->factors[outputs->count] = outputs->input;
                outputs->count++;
        }
        return ptr;
}

int main(int argc, char* argv[])
{
        int inputs[25];
        nums outputs[25];
        //void* ptr = &outputs;
        int i;
        for (i =0; i < argc-1; i++)
        {
                inputs[i] = atoi(argv[i+1]);
        }

        pthread_t thread[25];
        for (i = 0; i < argc-1; i++)
        {
                void* ptr = &outputs[i];
                outputs[i].input = inputs[i];
                pthread_create(&thread[i], NULL, findfactors, (void*)ptr);
        }

        for(i=0; i<argc-1; i++)
        {
                pthread_join(thread[i], NULL);
                printnums(outputs[i]);
        }

        return 0;
}
