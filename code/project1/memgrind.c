#ifndef TESTCASE
#define TESTCASE 1
#endif

#ifndef DEBUG_MODE
#define DEBUG_MODE 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{

    srand(time(NULL));

    switch (TESTCASE)
    {

    case 1:

        for (int i = 0; i < 120; i++)
        {
            void *ptr = malloc(1);
            free(ptr);
        }

        break;

    case 2:

        void *ptr_array[120];

        for (int i = 0; i < 120; i++)
        {
            void *ptr = malloc(1);
            ptr_array[i] = ptr;
        }

        for (int i = 0; i < 120; i++)
        {
            free(ptr_array[i]);
        }

        break;

    case 3:

        int n_allocations = 0;
        void *ptr_array[120];

        while (n_allocations < 120)
        {
            int choice = rand();

            if (choice == 0)
            {
                void *ptr = malloc(1);

                int i = 0;
                while (ptr_array[i] != NULL && i < 120)
                {
                    ptr_array[i] = ptr;
                }

                n_allocations++;
            }
            else
            {
                int j = 0;
                while (ptr_array[j] == NULL && j < 120)
                {
                    j++;
                }

                free(ptr_array[j]);

                n_allocations--;
            }
        }

        break;

    case 4:
        /* code */
        break;

    case 5:
        /* code */
        break;

    default:
        break;
    }
}