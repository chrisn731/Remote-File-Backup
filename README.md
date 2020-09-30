# Remote File Backup

A small project for transfering files from client to server for safe keeping.

To build on a machine, clone the directory and run `make both` to make both
the client and the server. Alternatively, use `make client` or `make server` to
make either client or server only.

If more data throughput is needed, access the `Shared/Helper.h` and change
the `STD_BUFF_SZ` to a higher number.
Similarly, to change the port, simply change the `PORT` to a value of your
liking. At compile time all necessary values will be set.
