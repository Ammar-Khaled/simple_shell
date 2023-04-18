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

### env

- To print all environment variables

## Build

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`
