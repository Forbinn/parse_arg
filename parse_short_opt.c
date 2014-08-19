/*
** parse_short_opt.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 19:50:33 2014 vincent leroy
** Last update Tue Aug 19 11:34:21 2014 vincent leroy
*/

#include <string.h>
#include <stdlib.h>

#include "parse_short_opt.h"

static int _get_opts_idx_for_opt(char short_opt, const opts options[])
{
    int i;

    for (i = 0; options[i].cb != NULL; ++i)
        if (options[i].short_opt == short_opt)
            return i;

    return -1;
}

static int _split_short_opt_in_opts(const char *short_opt, const opts options[], struct opts_tab *opts_required, struct opts_tab *opts_optional, struct opts_tab *opts_no, opt_error *error)
{
    int i, idx;
    int len = strlen(short_opt);

    memset(opts_required, 0, sizeof(struct opts_tab));
    memset(opts_optional, 0, sizeof(struct opts_tab));
    memset(opts_no, 0, sizeof(struct opts_tab));

    opts_required->opts = calloc(len, sizeof(struct opts_idx));
    opts_optional->opts = calloc(len, sizeof(struct opts_idx));
    opts_no->opts = calloc(len, sizeof(struct opts_idx));

    for (i = 0; short_opt[i] != '\0'; ++i)
    {
        idx = _get_opts_idx_for_opt(short_opt[i], options);
        if (idx == -1)
            return set_parse_error(error, -1, i, UNKNOWN_OPT);

        switch (options[idx].has_arg)
        {
            case NO_ARG:
                opts_no->opts[opts_no->size_opts].opt = &options[idx];
                opts_no->opts[opts_no->size_opts].char_idx = i;
                ++opts_no->size_opts;
                break;
            case OPTIONAL_ARG:
                opts_optional->opts[opts_optional->size_opts].opt = &options[idx];
                opts_optional->opts[opts_optional->size_opts].char_idx = i;
                ++opts_optional->size_opts;
                break;
            case REQUIRED_ARG:
                opts_required->opts[opts_required->size_opts].opt = &options[idx];
                opts_required->opts[opts_required->size_opts].char_idx = i;
                ++opts_required->size_opts;
                break;
        }
    }

    return 0;
}

static int _do_short_opt_cb(char *next_opt, const struct opts_tab *opt_required, const struct opts_tab *opt_optional, const struct opts_tab *opt_no, opt_error *error)
{
    bool next_opt_used = false;
    char *arg = NULL;
    int i;

    if (opt_required->size_opts > 0 && next_opt == NULL)
        return set_parse_error(error, -1, opt_required->opts[0].char_idx, ARG_NOT_PROVIDE);
    else if (opt_required->size_opts > 1)
        return set_parse_error(error, -1, opt_required->opts[0].char_idx, ARG_NOT_PROVIDE);

    if (opt_required->size_opts > 0)
    {
        if (!(opt_required->opts[0].opt->cb(next_opt, opt_required->opts[0].opt->user_data, true)))
            return set_parse_error(error, -1, opt_required->opts[0].char_idx, CALLBACK_ERROR);
        else
            next_opt_used = true;
    }

    for (i = 0; i < opt_optional->size_opts; ++i)
    {
        if (i == opt_optional->size_opts - 1 && !next_opt_used)
        {
            arg = next_opt;
            next_opt_used = true;
        }

        if (!(opt_optional->opts[i].opt->cb(arg, opt_optional->opts[i].opt->user_data, true)))
            return set_parse_error(error, -1, opt_optional->opts[i].char_idx, CALLBACK_ERROR);
    }

    for (i = 0; i < opt_no->size_opts; ++i)
        if (!(opt_no->opts[i].opt->cb(NULL, opt_no->opts[i].opt->user_data, true)))
            return set_parse_error(error, -1, opt_no->opts[i].char_idx, CALLBACK_ERROR);

    return next_opt_used && next_opt != NULL ? 2 : 1;
}

int parse_short_opt(const char *short_opt, char **next_opt, const opts options[], opt_error *error)
{
    int ret;
    struct opts_tab opt_required;
    struct opts_tab opt_optional;
    struct opts_tab opt_no;

    ret = _split_short_opt_in_opts(short_opt, options, &opt_required, &opt_optional, &opt_no, error);
    if (ret != -1)
    {
        ret = 0;
        char *arg = next_opt[0];
        if (arg != NULL && arg[0] == '-')
        {
            if (arg[1] == '-' && arg[2] == '\0')
            {
                arg = next_opt[1];
                ret = 1;
            }
            else
                arg = NULL;
        }

        ret += _do_short_opt_cb(arg, &opt_required, &opt_optional, &opt_no, error);
    }

    free(opt_required.opts);
    free(opt_optional.opts);
    free(opt_no.opts);

    return ret;
}
