# MiniShell

```
 ███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗
 ████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║
 ██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║
 ██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║
 ██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
 ╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
```

> *As beautiful as a shell. Our own little bash.*

[![42 School](https://img.shields.io/badge/42-School-000000?style=flat-square&logo=42&logoColor=white)](https://42.fr)
![C](https://img.shields.io/badge/C-99-blue?style=flat-square&logo=c&logoColor=white)
![Complexity](https://img.shields.io/badge/complexity-high-red?style=flat-square)

---

## About

MiniShell is a simplified recreation of bash, built from scratch in C. This is one of the most challenging projects in the 42 curriculum. It covers lexing, parsing, process management, file descriptor manipulation, and the inner workings of a UNIX shell.

The goal: type a command, and it works like you'd expect in bash.

## Features

```
 ┌───────────────────────────────────────────────────────────┐
 │  minishell$                                               │
 │                                                           │
 │  ✓ Command execution with absolute/relative paths         │
 │  ✓ PATH resolution                                        │
 │  ✓ Pipes                    cmd1 | cmd2 | cmd3            │
 │  ✓ Redirections             > >> < <<                     │
 │  ✓ Environment variables    $VAR, $?                      │
 │  ✓ Single & double quotes   'literal' "expand $VAR"       │
 │  ✓ Signal handling          ctrl-C, ctrl-D, ctrl-\        │
 │  ✓ Exit status              $?                            │
 │  ✓ Here-doc                 << DELIMITER                  │
 │                                                           │
 └───────────────────────────────────────────────────────────┘
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
├── src/
│   ├── lexer/          # Tokenization
│   ├── parser/         # AST construction
│   ├── executor/       # Command execution
│   ├── builtins/       # Built-in commands
│   ├── signals/        # Signal handlers
│   └── utils/          # Helpers
├── include/
├── Makefile
└── README.md
```

---

## Authors

[lmonsat](https://profile.intra.42.fr/users/lmonsat)
[torsini](https://profile.intra.42.fr/users/torsini)

## 42 School

This project was developed as part of the school's curriculum (https://42luxembourg.lu/).
