# Debugging Multi-Processing Programs Using GDB


## Attaching a running process (first method)

1. In the children processes C-files, include the following code:

```c
#ifdef _DEBUG_
sleep(60);
#endif
```

2. Compile your code using the gcc compiler and option -g and define the debugging flag as follows:

```bash
gcc -g -D_DEBUG_ file.c -o file
```

2. Open as many terminal windows as you have processes to dedug.

3. Run the command "sudo gdb" without arguments in the terminal windows.

4. Run the parent process under gdb and provide necessary arguments (e.g. run 5 1).

5. In a terminal window, execute the following command:

```bash
ps -ef | grep file
```

Replace file by the process name you want to debug.

6. In each terminal window where you executed the gdb command, execute the following:

gdb> `attach pid`

Where pid is the process you want to debug

7. Wait for the processes to wake up from the 60 second sleep period. Once they wake up, they will be suspended.

8. Place breakpoints in the process file you're debugging as follows:


gdb> `b child.c:50`

In the above command, I am placing a breakpoint at line 50 in the file child.c.

## Debugging under the same GDB instance (second method)

a. Compile the C-file using the -g option of the gcc but no need to define the _DEBUG_ variable.

b. run the command: 

```bash
gdb parent
```

c. before doing a run under gdb, run the following command under gdb:

gdb> `set detach-on-fork off`

The above command will hold both processes (parent and child) under the control of gdb. By default, the parent process will be debugged as usual and the child will be held suspended.

c. Under gdb, if you wish to follow the child process, run the following command under gdb:


gdb> `set follow-fork-mode child`
gdb> `set follow-exec-mode new`


The above will make the gdb debugger follow the child process while the parent process is suspended.

d. To quit debugging one of the forked processes, you can either detach from it by executing:

gdb> `detach inferiors`
