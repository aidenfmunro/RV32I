#include "api.h"
#include "lib.h"

int main()
{
    char buf[64];

    int nread = read(0, buf, sizeof(buf));

    if (nread <= 0)
    {
        exit(1);
    }

    int pos = 0;
    int a = parse_int(buf, nread, &pos);

    float fa = (float) a;

    float res = fa / 2;

    int ires = (int) res;

    write_int_ln(ires);

    exit(0);
}
