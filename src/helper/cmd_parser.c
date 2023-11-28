#include <bits/getopt_ext.h>
#include <getopt.h>

struct option* optrule()
{
    static struct option optrule[] = {
        {"addr", 1, required_argument, 'a'},
        {"port", 1, required_argument, 'p'},
        {0, 1, 0, 0}
    };

    return optrule;
}

