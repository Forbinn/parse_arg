/*
** main.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 14:00:31 2014 vincent leroy
** Last update Tue Aug 19 15:48:57 2014 vincent leroy
*/

#include <stdio.h>
#include <sysexits.h>
//#include <error.h>

#include "parse_arg.h"

void usage(const char *av0)
{
    fprintf(stderr, "Usage: %s [-a [__argument__]] [--list __argument__] [-h|--help]\n", av0);
    fprintf(stderr, "\t-a           A short option with an optional argument\n");
    fprintf(stderr, "\t--list       A long option with a required argument\n");
    fprintf(stderr, "\t-h,--help    Display this help and exit\n");
}

static bool _h_arg(char *arg, void *user_data, bool short_arg)
{
    (void)arg;

    printf("'%s' option\n", short_arg ? "h" : "help");

    usage(user_data);
    return true;
}

static bool _a_arg(char *arg, void *user_data, bool short_arg)
{
    (void)user_data;
    (void)short_arg;

    printf("'a' option");
    if (arg != NULL)
        printf(" with option '%s'", arg);
    printf("\n");

    return true;
}

static bool _l_arg(char *arg, void *user_data, bool short_arg)
{
    (void)user_data;
    (void)short_arg;

    printf("'list' option");
    if (arg != NULL)
        printf(" with option '%s'", arg);
    printf("\n");

    return true;
}

int scan_arg(int *ac, char ***av)
{
    opt_error error;
    opts options[] = {
        {'h', "help", NO_ARG, &_h_arg, (*av)[0]},
        {'a', NULL, OPTIONAL_ARG, &_a_arg, NULL},
        {'\0', "list", REQUIRED_ARG, &_l_arg, NULL},
        {'\0', NULL, NO_ARG, NULL, NULL}
    };

    if (parse_arg(ac, av, options, &error) == -1)
    {
        switch (error.err_type)
        {
            case UNKNOWN_OPT:
                if (error.is_short_arg)
                    fprintf(stderr, "Unknown option: '%c'\n", (*av)[error.argv_idx][error.idx]);
                else
                    fprintf(stderr, "Unknown option: '%s'\n", (*av)[error.argv_idx] + error.idx);
                break;
            case ARG_NOT_PROVIDE:
                if (error.is_short_arg)
                    fprintf(stderr, "Argument not provide for option: '%c'\n", (*av)[error.argv_idx][error.idx]);
                else
                    fprintf(stderr, "Argument not provide for option: '%s'\n", (*av)[error.argv_idx] + error.idx);
                break;
            case CALLBACK_ERROR:
                if (error.is_short_arg)
                    fprintf(stderr, "Callback error for option: '%c'\n", (*av)[error.argv_idx][error.idx]);
                else
                    fprintf(stderr, "Callback error for option: '%s'\n", (*av)[error.argv_idx] + error.idx);
                break;
            default:
                break;
        }

        return EX_SOFTWARE;
    }

    return 0;
}

int main(int ac, char **av)
{
    int ret = scan_arg(&ac, &av);
    int i;
    for (i = 0; i < ac; ++i)
        printf("av[%d] = '%s'\n", i, av[i]);

    if (ret != 0)
        return ret;

    return EX_OK;
}
