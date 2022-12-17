# Linking Pthread

The pthread library is not linked by default. You need to add the -lpthread flag to the gcc command.

```zsh
gcc file.c -o file -lpthread
```