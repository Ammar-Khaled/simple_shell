# Simple Shell

## Build

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`

## Usage

- interactive mode

```bash
$ ./hsh
```

- commands as an argument

```bash
$ ./hsh "/bin/ls"
```

- commands from standard input stream

```bash
$ echo "/bin/ls" | hsh
```

## Builtin Commands

### exit [status]

- To exit the shell with optional status code, defaults to zero

### env [variable_name]

- To print all environment variables

- Or provide one argument to print its value if exists

### setenv [variable_name] [variable_value]

- To set or modify environment variable

### unsetenv [variable_name]

- To unset an exist environment variable

### alias [name[='value'] ...]

- Make an alias name for long commands
*`alias`: Prints a list of all aliases, one per line, in the form `name='value'`
*`alias name [name2 ...]`: Prints the aliases name, name2, etc 1 per line, in the form name='value'
*`alias name='value' [...]`: Defines an alias for each `name` whose `value` is given. If name is already an alias, replaces its value with `value`

## Environment Variables

- PWD: current working directory
- OLDPWD: last working directory
- PROMPT: the shell prompt string
- SHELL: the shell name
- SHELL_EXEC: the shell executable file name
- status: the last process exist status
- ?: shorthand for --status--

## Tests

### Utils

- test all implemented cstring functions

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 -g utils* _realloc.c tests/
utils.c && ./a.out`
