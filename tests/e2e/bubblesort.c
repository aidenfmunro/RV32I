#include "api.h"
#include "lib.h"

static void bubblesort(int* a, long n)
{
    if (n <= 1)
        return;

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - 1 - i; ++j)
        {
            if (a[j] > a[j + 1])
            {
                int tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }
}

int main(void)
{
    char buf[512];
    int nread = read(0, buf, sizeof(buf));

    if (nread <= 0)
    {
        exit(1);
    }

    int pos = 0;
    int n = parse_int(buf, nread, &pos);

    if (n <= 0 || n > 64)
    {
        exit(1);
    }

    int arr[64];

    for (int i = 0; i < n; ++i)
    {
        arr[i] = parse_int(buf, nread, &pos);
    }

    bubblesort(arr, n);

    for (int i = 0; i < n; ++i)
    {
        write_int_space(arr[i]);
    }

    write(1, "\n", 1);

    exit(0);
}
