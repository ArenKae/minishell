# Minishell

<p align="center">
  <img src="https://github.com/ArenKae/ArenKae/blob/main/42%20badges/minishelle.png" alt="Minishell 42 project badge"/>
</p>

## As beautiful as a shell
The existence of shells is linked to the very existence of IT. Soon after the invention of the firsts microcomputers, all developers agreed that communicating with a machine using aligned 1/0 switches was seriously irritating. It was only logical that they came up with the idea of creating a software to communicate with a computer using interactive lines of commands in a language somewhat close to the human language.

Thanks to Minishell, we'll have the opportunity to travel through time and confront problems people faced back when Windows didn’t exist. This team project is an implementation of a basic shell program in C. It implements redirections and pipes, as well as environment variable expansions and a few builtin commands.

<p align="center">
  <img src="https://github.com/ArenKae/ArenKae/blob/main/screens/Minishell.gif" alt="mimishell demo gif">
</p>

## Status
✅ Completed on: 22/12/2023
</br>
🏆 Grade: 101/100

## Usage

Clone this repository and ```cd``` into it. Use ```make``` to compile, then launch the program with :
```
./minishell
```
A prompt will appear. You may enter your commands to be executed.

## Supported Features

Minishell is a miniature shell program based on Bash. Minishell supports:
* Prompt display
* Command history (up and down arrows)
* System executables available from the environment (`ls`, `cat`, `grep`, etc.)
* Local executables (`./minishell`)
* Builtin commands :
  * `echo` (and option `-n`)
  * `cd` (with only a relative or absolute path)
  * `pwd` (no options)
  * `export` (no options)
  * `unset` (no options)
  * `env` (no options or arguments)
  * `exit` (with exit number but no other options) 
* Pipes `|` which redirect output from one command to input for the next
* Redirections:
  * `>` redirects output
  * `>>` redirects output in append mode
  * `<` redirects input
  * `<< DELIMITER` (heredoc) displays a new prompt, reads user input until reaching `DELIMITER`, redirects user input to command input (does not update history)
* Environment variables (i.e. `$USER` or `$VAR`) that expand to their values.
  * `$?` expands to the exit status of the most last executed command.
* User keyboard signals:
  * `ctrl-c` displays a new prompt line.
  * `ctrl-d` exits minishell
  * `ctrl-\` does nothing

---

This project was made in collaboration with keschouf.
