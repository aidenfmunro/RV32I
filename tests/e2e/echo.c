#include "api.h"

int main( int argc, char* argv[])
{
    char data[1024];

    return write( 1, data, read( 0, data, sizeof(data)));
}
