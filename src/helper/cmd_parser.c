
#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>
#include "header/cmd_parser.h"

struct option* optrule()
{
    static struct option optrule[] = {
        {"addr", 1, NULL, 'a'},
        {"port", 1, NULL, 'p'},
        {NULL, 0, 0, 0}
    };

    return optrule;
}

void optparse(int argc, char *argv[], struct cmd_prop *cmd_prop)
{
    int char_ret;
    int longindex = 0;
    
    while (1)
    {
        char_ret = getopt_long(argc, argv, "a:p", optrule(), &longindex);

        if (char_ret == -1)
        {
            break;
        }

        switch (char_ret) {
            case 'a':
                cmd_prop->server_addr = optarg;
                cmd_prop->cmd_prop_status.server_addr = 1;
                break;
            case 'p':
                cmd_prop->port = atoi(optarg);
                cmd_prop->cmd_prop_status.port = 1;
                break;
        }
        
    }
}