/*
** parse_error.c for parse_arg in /home/leroy_v/Perso/parse_arg
**
** Made by vincent leroy
** Login  <leroy_v@epitech.eu>
**
** Started on  Mon Aug 18 18:12:08 2014 vincent leroy
** Last update Mon Aug 18 18:48:16 2014 vincent leroy
*/

#include <stdlib.h>

#include "parse_error.h"

int set_parse_error(opt_error *error, int argv_idx, int idx, error_type err_type)
{
    if (error == NULL)
        return -1;

    if (argv_idx != -1)
        error->argv_idx = argv_idx;
    if (idx != -1)
        error->idx = idx;
    if (err_type != DONT_CHANGE_ERROR_TYPE)
        error->err_type = err_type;

    return -1;
}

void clear_parse_error(opt_error *error)
{
    if (error == NULL)
        return ;

    error->argv_idx = -1;
    error->idx = -1;
    error->err_type = NO_ERROR;
}
