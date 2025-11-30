#include "api.h"
#include "lib.h"

static int isqrt(int n)
{
    if (n <= 0)
        return 0;

    int x = 0;

    while ((x + 1) * (x + 1) <= n)
    {
        ++x;
    }

    return x;
}

int main(void)
{
    char buf[64];
    int nread = read(0, buf, sizeof(buf));

    if (nread <= 0)
    {
        exit(1);
    }

    int pos = 0;
    int n = parse_int(buf, nread, &pos);
    int r = isqrt(n);

    write_int_ln(r);

    exit(0);
}
