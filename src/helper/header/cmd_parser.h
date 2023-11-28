#ifndef _CMD_PARSER_H
#define _CMD_PARSER_H

struct cmd_prop_status
{
    int             server_addr;
    int             port;
};

struct cmd_prop
{
    char*           server_addr;
    int             port;

    struct cmd_prop_status cmd_prop_status;
};

void optparse(int argc, char *argv[], struct cmd_prop *cmd_prop);

#endif