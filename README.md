# Simple Shell

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

### exit

- To exit the shell

### env [variable_name]

- To print all environment variables

- Or provide one argument to print its value if exists

## Build

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`
