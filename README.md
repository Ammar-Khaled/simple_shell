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

### env [variable_name] [variable_value]

- To print all environment variables

- Or provide one argument to print its value if exists

- Or provide two arguments (name, value) to set and overwrite

## Environment Variables

- PWD: current working directory
- OLDPWD: last working directory
- PROMPT: the shell prompt string
- SHELL: the shell name
- SHELL_EXEC: the shell executable file name
- status: the last process exist status
- ?: shorthand for --status--

## Build

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`
