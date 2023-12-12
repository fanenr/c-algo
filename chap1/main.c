#include "../common.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static void solution1(int n, int *nums);
static void solution2(int n, int *nums);
static void solution3(int n, int *nums);

int
main(void)
{
    int n;
    int *nums;

    scanf("%d", &n);
    nums = (int *)malloc(sizeof(int) * n);
    assert(nums != NULL);
    for (int i = 0; i < n; i++)
        scanf("%d", &nums[i]);

    solution1(n, nums);
    solution2(n, nums);
    solution3(n, nums);
}

static void
solution1(int n, int *nums)
{
    TIME_ST();

    int temp;
    int k = n / 2;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (nums[j] > nums[j + 1]) {
                temp = nums[j];
                nums[j] = nums[j + 1];
                nums[j + 1] = temp;
            }

    TIME_ED();

    printf("val: %d\n", nums[k]);
    printf("time: %.3f\n\n", TIME_VAL());
}

static int
down_order(const void *a, const void *b)
{
    const int *pa = a;
    const int *pb = b;
    return *pa - *pb;
}

static void
solution2(int n, int *nums)
{
    TIME_ST();

    int k = n / 2;
    qsort(nums, k, sizeof(int), down_order);

    for (int i = k; i < n; i++)
        for (int j = k - 1; j >= 0; j--)
            if (nums[i] < nums[j]) {
                if (j < k - 1)
                    nums[j + 1] = nums[i];
                break;
            } else if (j < k - 1) {
                nums[j + 1] = nums[j];
            }

    TIME_ED();

    printf("val: %d\n", nums[k]);
    printf("time: %.3f\n\n", TIME_VAL());
}

static void
solution3(int n, int *nums)
{
    TIME_ST();

    int k = n / 2;
    qsort(nums, n, sizeof(int), down_order);

    TIME_ED();

    printf("val: %d\n", nums[k]);
    printf("time: %.3f\n", TIME_VAL());
}
