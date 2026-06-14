op-shell
========

What this is
------------
op-shell is a command-line shell written in C, built for an Operating
Systems course. The same source code runs in two different places:

  1. In a real terminal (Linux/macOS): clone the repository and run
     "make && ./shell". There the shell uses fork() and execvp(), the
     way a real shell launches processes.

  2. Here, in the browser: the C was compiled to WebAssembly with
     Emscripten and runs over a virtual, in-memory filesystem. This is
     the version you are exploring right now.

This web version is a demo: it lets you try the project without
installing anything. There is no server and no backend, everything you
type is run by the same C code inside your browser tab. If you want to
see the source, it is all here:

  https://github.com/tomasleote/op-shell


Getting started (quick guide)
-----------------------------
If you have never used a shell, try these commands in this order. Type
one at a time and press Enter.

  ls               list the files and folders in the current folder
  ls projects      list the contents of the "projects" folder
  cd projects      enter the "projects" folder
  pwd              show which folder you are in now
  cat readme.txt   print the contents of a file to the screen
  cd ..            go back to the previous folder
  clear            clear the screen
  help             show all available commands

Suggested path: start with "cat about-me.txt", then "cat contacts.txt",
and finally "cd projects" and read the files inside with "cat".


Commands available in this web version
--------------------------------------
  help             list of commands and help
  ls [folder]      list the contents of a folder
  cd [folder]      change folder (cd .. goes up one level)
  pwd              show the path of the current folder
  cat <file>       print the contents of a file
  echo [text]      write text back to the screen
  mkdir <folder>   create a new folder
  touch <file>     create an empty file
  rm [-r] <target> remove a file (or folder, with -r)
  clear            clear the screen
  status           show the exit code of the last command
  exit             end the session (in the browser it just says goodbye)


Tip
---
Shells hide files whose name starts with a dot. The normal "ls" does not
show them. If you are curious, there is something waiting for those who
know this: try "cat .secret.txt".
