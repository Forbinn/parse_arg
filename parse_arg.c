/*
** parse_arg.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 14:24:44 2014 vincent leroy
** Last update Tue Aug 19 14:05:29 2014 vincent leroy
*/

#include <stdlib.h>
#include <string.h>

#include "parse_arg.h"
#include "parse_short_opt.h"
#include "parse_long_opt.h"

static void _remove_option_in_av(int *ac, char ***av, int *idx_in_av_for_remove, int size_of_tab)
{
    int i;
    int j;

    for (i = 0; i < size_of_tab; ++i)
    {
        for (j = idx_in_av_for_remove[i]; j < *ac; ++j)
            (*av)[j] = (*av)[j + 1];
        --(*ac);

        for (j = i + 1; j < size_of_tab; ++j)
            --idx_in_av_for_remove[j];
    }
}

int _do_parse(char **argv, int i, int off, const opts options[], opt_error *error, int *idx_in_av_for_remove, int *idx_in_tab, int (*parse_func)(const char*, char**, const opts[], opt_error*))
{
    int nb_arg_parsed;
    int j;

    nb_arg_parsed = (*parse_func)(argv[i] + off, argv + i + 1, options, error);
    if (nb_arg_parsed == -1)
    {
        free(idx_in_av_for_remove);
        return set_parse_error(error, i, error ? error->idx + off : -1, DONT_CHANGE_ERROR_TYPE);
    }

    for (j = 0; j < nb_arg_parsed; ++j)
    {
        idx_in_av_for_remove[*idx_in_tab] = i + j;
        ++(*idx_in_tab);
    }

    return nb_arg_parsed;
}

int parse_arg(int *ac, char ***av, const opts options[], opt_error *error)
{
    int nb_arg_parsed = 0;
    int i;
    int *idx_in_av_for_remove;
    int idx_in_tab = 0;
    char **argv = *av;

    idx_in_av_for_remove = calloc(*ac, sizeof(int));
    clear_parse_error(error);

    for (i = 1; i < *ac; i += nb_arg_parsed)
    {
        if (argv[i][0] == '-' && argv[i][1] != '-' && argv[i][1] != '\0')
        {
            error->is_short_arg = true;
            nb_arg_parsed = _do_parse(argv, i, 1, options, error, idx_in_av_for_remove, &idx_in_tab, &parse_short_opt);
            if (nb_arg_parsed == -1)
                return -1;
        }
        else if (argv[i][0] == '-' && argv[i][1] == '-')
        {
            if (argv[i][2] == '\0')
            {
                nb_arg_parsed = 2;
                continue;
            }

            error->is_short_arg = false;
            nb_arg_parsed = _do_parse(argv, i, 2, options, error, idx_in_av_for_remove, &idx_in_tab, &parse_long_opt);
            if (nb_arg_parsed == -1)
                return -1;
        }
        else
        {
            nb_arg_parsed = 1;
            continue;
        }
    }

    _remove_option_in_av(ac, av, idx_in_av_for_remove, idx_in_tab);
    free(idx_in_av_for_remove);
    return 0;
}