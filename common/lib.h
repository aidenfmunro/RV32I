#pragma once

#include "api.h"


// Skip ASCII whitespace
static int skip_ws(const char* buf, int len, int pos)
{
    while (pos < len)
    {
        char c = buf[pos];

        if (c != ' ' && c != '\n' && c != '\t' && c != '\r')
            break;

        ++pos;
    }

    return pos;
}

static int parse_int(const char* buf, int len, int* pos)
{
    int i = skip_ws(buf, len, *pos);
    int sign = 1;

    if (i < len && buf[i] == '-')
    {
        sign = -1;
        ++i;
    }

    int v = 0;
    int seen_digit = 0;

    for (; i < len; ++i)
    {
        char c = buf[i];

        if (c < '0' || c > '9')
            break;

        seen_digit = 1;

        v = v * 10 + (c - '0');
    }

    *pos = i;

    if (!seen_digit)
        return 0; // simple error behaviour

    return sign * v;
}

static void write_int_core(int x, char* buf, int* out_len)
{
    int i = 0;
    if (x == 0)
    {
        buf[i++] = '0';
    }
    else
    {
        if (x < 0)
        {
            buf[i++] = '-';
            x = -x;
        }

        char tmp[32];
        int j = 0;

        while (x > 0)
        {
            tmp[j++] = (char)('0' + (x % 10));
            x /= 10;
        }

        while (j > 0)
        {
            buf[i++] = tmp[--j];
        }
    }

    *out_len = i;
}

static void write_int_ln(int x)
{
    char buf[32];
    int n;
    write_int_core(x, buf, &n);
    buf[n++] = '\n';
    write(1, buf, n);
}

static void write_int_space(int x)
{
    char buf[32];
    int n;
    write_int_core(x, buf, &n);
    buf[n++] = ' ';
    write(1, buf, n);
}
