/*
** main.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 14:00:31 2014 vincent leroy
** Last update Thu Apr 30 15:21:18 2015 vincent leroy
*/

#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
//#include <error.h>

#include "parse_arg.h"

void usage(const char *av0)
{
    fprintf(stderr, "Usage: %s [-h|--help] [-a|--all] [-A|--allmost-all] [--author] [-b|--escape] [--block-size SIZE] [-c] [--color [COLOR]] [-I|--ignore ARG] [-l] other_arg...\n", av0);
    fprintf(stderr, "\t-h, --help\n");
    fprintf(stderr, "\t-a, --all\n");
    fprintf(stderr, "\t-A, --allmost-all\n");
    fprintf(stderr, "\t--author\n");
    fprintf(stderr, "\t-b, --escape\n");
    fprintf(stderr, "\t--block-size=SIZE\n");
    fprintf(stderr, "\t-c\n");
    fprintf(stderr, "\t--color[=COLOR]\n");
    fprintf(stderr, "\t-I, --ignore=ARG\n");
    fprintf(stderr, "\t-l\n");

    exit(EX_USAGE);
}

static bool _h_cb(const char *option, const char *arg, void *user_data)
{
    (void)arg;

    printf("'%s' option\n", option);

    usage(user_data);
    return false;
}

static bool _option_cb(const char *option, const char *arg, void *user_data)
{
    (void)user_data;

    printf("'%s' option", option);
    if (arg)
        printf(" with argument '%s'", arg);
    printf("\n");

    return true;
}

int scan_arg(int *ac, char ***av)
{
    opt_error error;
    opts options[] = {
        {'h',   "help",         NO_ARG,         &_h_cb,         (*av)[0]},
        {'a',   "all",          NO_ARG,         &_option_cb,    NULL},
        {'A',   "allmost-all",  NO_ARG,         &_option_cb,    NULL},
        {'\0',  "author",       NO_ARG,         &_option_cb,    NULL},
        {'b',   "escape",       NO_ARG,         &_option_cb,    NULL},
        {'\0',  "block-size",   REQUIRED_ARG,   &_option_cb,    NULL},
        {'c',   NULL,           NO_ARG,         &_option_cb,    NULL},
        {'\0',  "color",        OPTIONAL_ARG,   &_option_cb,    NULL},
        {'I',   "ignore",       REQUIRED_ARG,   &_option_cb,    NULL},
        {'l',   NULL,           NO_ARG,         &_option_cb,    NULL},
        {'\0',  NULL,           NO_ARG,         NULL,           NULL}
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
            case INVALID_ARG:
                fprintf(stderr, "Error: duplicated %s argument at index %d", error.is_short_arg ? "short" : "long", error.idx);
                if (error.is_short_arg)
                    fprintf(stderr, " (%c)\n", options[error.idx].short_opt);
                else
                    fprintf(stderr, " (%s)\n", options[error.idx].long_opt);
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
