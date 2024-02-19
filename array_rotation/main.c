#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef int (*myArray)[4];
void print_array(myArray array)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}

int isEnd(myArray array, int i, int j)
{
    if(array[i][j] == 0)
    {
        return 0;
    }
    if ((i + 1 >= 3 || array[i + 1][j] != 0) && (i - 1 < 0 || array[i - 1][j] != 0) && (j + 1 >= 4 || array[i][j + 1] != 0) && (j - 1 < 0 || array[i][j - 1] != 0))
    {
        return 1;
    }
    return 0;
}

void offset(myArray array, int i, int j, int *offset_i, int *offset_j)
{
    if (*offset_i != 0)
    {
        if (i + *offset_i >= 3 || i + *offset_i < 0 || array[i + *offset_i][j] != 0)
        {
            if (*offset_i > 0)
            {
                *offset_j = -1;
            }
            else
            {
                *offset_j = 1;
            }
            *offset_i = 0;
        }
    }
    else if (*offset_j != 0)
    {
        if (j + *offset_j >= 4 || j + *offset_j < 0 || array[i][j + *offset_j] != 0)
        {
            if (*offset_j > 0)
            {
                *offset_i = 1;
            }
            else
            {
                *offset_i = -1;
            }
            *offset_j = 0;
        }
    }
}

int main(void)
{
    int array[3][4] = {0};

    int offset_i = 0;
    int offset_j = 1;
    int i = 0, j = 0;
    int num = 1;
    while (1)
    {
        if (isEnd(array, i, j))
        {
            break;
        }
        else
        {
            offset(array, i, j, &offset_i, &offset_j);
        }
        array[i][j] = num++;
        i += offset_i;
        j += offset_j;
    }

    print_array(array);
    return 0;
}
