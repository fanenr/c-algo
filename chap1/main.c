#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../common.h"

static void solution1(int n, int *nums);
static void solution2(int n, int *nums);

int main(void)
{
    int *nums;
    int n;

    scanf("%u", &n);
    nums = (int *)malloc(sizeof(int) * n);
    assert(nums != NULL);
    for (int i = 0; i < n; i++)
        scanf("%d", &nums[i]);

    solution1(n, nums);
    solution2(n, nums);
}

static void solution1(int n, int *nums)
{
    TIME_ST();

    int temp;
    int k = n / 2 - 1;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (nums[j] > nums[j + 1]) {
                temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }

    printf("val: %d\n", nums[k]);

    TIME_ED();

    printf("time: %.3f\n", TIME_VAL());
}

static void solution2(int n, int *nums)
{
    TIME_ST();

    int k = n / 2 - 1;

    TIME_ED();

    printf("time: %.3f\n", TIME_VAL());
}
