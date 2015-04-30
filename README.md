parse_arg
=========

Library to facilitate parsing of arguments passed to the main function.
This library contains only **one** function:

```c
int parse_arg(int *ac, char ***av, const opts options[], opt_error *error);
```

This function will **remove** the argument parsed **only** if no error occurred.
Example:

```c
// l : NO_ARG
// long_option : REQUIRED_ARG

./your_program -l arg file --long_option=arg arg2
```

After the parsing you will have in the **ac** argument the value 4 and in the **av** argument the value: ["./your_program", "arg", "file", "arg2"]

opts structure
--------------

The *opts* structure is used to describe the parameters expected by the program.<br>
It has the following field:

- **short_opt** : contains the value of the short form of the option (e.g. 'h' for help option).
- **long_opt** : contains the value of the long form of the option (e.g. 'help' for help option).
- **has_arg** : enumeration that contains one of the three value describe after.
- **cb** : function call when the option is found in the main parameter.
- **user_data** : user data pass to the callback.

argument enumeration
--------------------

The *argument* enumeration is used to tell if the option take an argument or not.
It has the following value:

| Enum value   | Decimal value |                         Description  |
| :----------- | -------------:| -----------------------------------: |
| NO_ARG       |             0 | The option don't takes arguments     |
| OPTIONAL_ARG |             1 | The option **can** take an argument  |
| REQUIRED_ARG |             2 | The option **must** take an argument |

Callback
--------

The callback (the **cb** field in the *opts* structure) must have the following prototype:

```c
bool callback(const char *option, const char *arg, void *user_data);
```

If the callback return false then the parsing stop and the error is set to **CALLBACK_ERROR**.

The first parameter (i.e. *option*) is set to the option that triggered the call of the callback.<br>
The second parameter (i.e. *arg*) is set to the argument of the option following the rules describe by the *argument* enumeration.<br>
The third parameter (i.e. *user_data*) is set to the value of the **user_data** field in the *opts* structure.<br>

The *option* parameter is only valid during the call of the callback i.e. If you want to keep its value after the return of the callback you need to call a function like **strdup**.

### Warning ###

```c
opts options[] = {
    {'a', NULL, NO_ARG, &a_option, NULL},
    {'b', NULL, OPTIONAL_ARG, &b_option, NULL},
    {'c', NULL, OPTIONAL_ARG, &c_option, NULL},
    {'d', NULL, REQUIRED_ARG, &d_option, NULL},
    {'\0', NULL, NO_ARG, NULL, NULL}
};

// [1] -a -d arg -c
// [2] -bc arg
// [3] -cb arg
// [4] -dcb arg
// [5] -d -- -arg
```

- **[1]**: In this case the calling order of the callback is guaranteed: *a_option* then *d_option* and *c_option*.
- **[2]**: The argument *arg* will be given to the *c_option* callback.
- **[3]**: The argument *arg* will be given to the *b_option* callback.
- **[4]**: The argument *arg* will be given to the *d_option* callback and not to *b_option* because the *d* option **required** an argument.
- **[5]**: The argument *-arg* will be given to the *d_option* callback even if the argument start with a *-*.

Error system
------------

If an error occur during the parsing the **parse_arg** function will return -1 and the **error** parameter (If it is non-NULL) will describe the error.<br>
The *opt_error* structure contains several field that serve to describe and localized the error:

- **argv_idx** : contains an index in the *argv* array where the error occured.
- **idx** : contains an index in the *argv[argv_idx]* string which designed the wrong character.
- **is_short_arg** : is **true** if the wrong parameter was a short parameter.
- **err_type** : enumeration that contains one of the four value describe after.

In the case of the **INVALID_ARG** error the *argv_idx* field is not used.<br>
The *idx* field is used to localised the error in the *options* array.<br>
The *is_short_arg* tell if the duplicated argument is a short or a long argument in the *options* array.

error_type enumeration
----------------------

The *error_type* enumeration is used to describe to type of error that occured.<br>
It has the follwing value:

| Enum value      | Decimal value |                                                                Description       |
| :-------------- | -------------:| -------------------------------------------------------------------------------: |
| NO_ERROR        |             0 | No error has occured                                                             |
| UNKNOWN_OPT     |             1 | The argument is not known                                                        |
| ARG_NOT_PROVIDE |             2 | If the **has_arg** field is equal to **REQUIRED_ARG** and no argument is provide |
| CALLBACK_ERROR  |             3 | The callback has return **false**                                                |
| INVALID_ARG     |             4 | The opts structure contains duplicate arguments                                  |

Example
=======

```c
void usage(char *av0)
{
    fprintf(stderr, "Usage: %s [-h|--help] [--useless=ARG] more_arg...\n", av0);
    fprintf(stderr, "\t-h,--help    Display this help and exit\n");
    fprintf(stderr, "\t--useless    A long option which take an argument");
}

bool h_option(const char *option, const char *arg, void *user_data)
{
    (void)arg;
    (void)option;

    usage(user_data);
    return true;
}

bool useless_arg(const char *option, const char *arg, void *user_data)
{
    (void)user_data;
    (void)option;

    return strcmp(arg, "argument") == 0;
}

int main(int ac, char **av)
{
    opts options[] = {
        {'h',   "help",     NO_ARG,         &h_option,      av[0]},
        {'\0',  "useless",  REQUIRED_ARG,   &useless_arg,   NULL},
        {'\0',  NULL,       NO_ARG,         NULL,           NULL}
    };

    if (parse_arg(&ac, &av, options, NULL) == -1)
        return 1;

    return 0;
}
```

You can see a full example in the main.c file.
