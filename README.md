The sole purpose of this program is to log the pid name and cmdline in a loop, so when I kill some process to use `grep -i 'process name or pid' /tmp/log` to retrieve what I have killed.

The program is only useful if your init system is **not** systemd as they have journalctl.

By default it will log everything in **/tmp/log** every 1 second or until press ==CTRL + C==

# Installation

```c
$ make
# make install
```
