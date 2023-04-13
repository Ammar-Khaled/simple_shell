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

## Build

`gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh`
