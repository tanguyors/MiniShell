# MiniShell

```
 ââââ   âââââââââââ   âââââââââââââââââ  ââââââââââââââ     âââ
 âââââ âââââââââââââ  âââââââââââââââââ  ââââââââââââââ     âââ
 ââââââââââââââââââââ ââââââââââââââââââââââââââââ  âââ     âââ
 âââââââââââââââââââââââââââââââââââââââââââââââââ  âââ     âââ
 âââ âââ âââââââââ ââââââââââââââââââââ  âââââââââââââââââââââââââââ
 âââ     âââââââââ  âââââââââââââââââââ  âââââââââââââââââââââââââââ
```

> *As beautiful as a shell. Our own little bash.*

[![42 School](https://img.shields.io/badge/42-School-000000?style=flat-square&logo=42&logoColor=white)](https://42.fr)
![C](https://img.shields.io/badge/C-99-blue?style=flat-square&logo=c&logoColor=white)
![Complexity](https://img.shields.io/badge/complexity-high-red?style=flat-square)

---

## About

**MiniShell** is a simplified recreation of `bash`, built from scratch in C. This is one of the most challenging projects in the 42 curriculum â it covers **lexing**, **parsing**, **process management**, **file descriptor manipulation**, and the inner workings of a UNIX shell.

The goal: type a command, and it works like you'd expect in bash.

## Features

```
 âââââââââââââââââââââââââââââââââââââââââââââââââââââââââââ
 â  minishell$                                             â
 â                                                         â
 â  â Command execution with absolute/relative paths       â
 â  â PATH resolution                                      â
 â  â Pipes                    cmd1 | cmd2 | cmd3          â
 â  â Redirections             > >> < <<                   â
 â  â Environment variables    $VAR, $?                    â
 â  â Single & double quotes   'literal' "expand $VAR"     â
 â  â Signal handling          ctrl-C, ctrl-D, ctrl-\      â
 â  â Exit status              $?                          â
 â  â Here-doc                 << DELIMITER                â
 â                                                         â
 âââââââââââââââââââââââââââââââââââââââââââââââââââââââââââ
```

### Builtins

| Command | Description |
|---------|-------------|
| `echo` | Display text (supports `-n` flag) |
| `cd` | Change directory |
| `pwd` | Print working directory |
| `export` | Set environment variables |
| `unset` | Remove environment variables |
| `env` | Display environment |
| `exit` | Exit the shell |

## Build

```bash
make        # Build minishell
make clean  # Remove object files
make fclean # Full clean
make re     # Rebuild
```

> Requires `readline` library (`-lreadline`)

## Usage

```bash
./minishell
minishell$ echo "Hello World" | cat -e
Hello World$
minishell$ ls -la | grep src | wc -l
3
minishell$ export FOO="bar" && echo $FOO
bar
minishell$ < infile cat | sort > outfile
minishell$ exit
```

## Architecture

```
MiniShell/
âââ src/
â   âââ lexer/          # Tokenization
â   âââ parser/         # AST construction
â   âââ executor/       # Command execution
â   âââ builtins/       # Built-in commands
â   âââ signals/        # Signal handlers
â   âââ utils/          # Helpers
âââ include/
âââ Makefile
âââ README.md
```

---

*The shell is the programmer's forge. â 42 School*
