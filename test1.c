extern long read( int fd, char* data, long maxlen);
extern long write( int fd, const char* data, long len);
extern __attribute__((noreturn)) void exit( long status);

int main( int argc, char* argv[])
{
    int a = 1;
    int b = 2;

    if (a + b != 3)
        return 1;

    return 0;
}

