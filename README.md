# Simple Shell Project

## Usage:

first compile the program using:

```
./compile.sh
```

This will produce a `hsh` executable,  
you can run it in interactive mode:

```
./hsh
$ /bin/pwd
/home
$ ^C
```

or in non-interactive mode

```
echo "/bin/ls" | ./hsh
file1 file2 dir1 dir2
```

