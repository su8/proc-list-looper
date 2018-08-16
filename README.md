The sole purpose of this program is to log the pid name and cmdline in a loop, so when I kill some process to use `grep -i 'process name or pid' /tmp/log` to retrieve what I have killed.

The program is only useful if your init system is **not** systemd as they have journalctl.

By default it will log everything in **/tmp/log** every 1 second or until press ==CTRL + C==

# Usage

```bash
while true; do
  list
  sleep 1
done
```

# Installation

```c
$ make
# make install
```

# Kernel module

```bash
cd kernel-module
make -j4 # 4 cores to use in parallel compilation

# to test the module
sudo insmod list.ko

# to read it's output
dmesg

# to stop it
sudo rmmod list.ko
```

---

You can achieve something similar with python:

```python
import os

while True:
    pids = set(os.listdir("/proc"));
    for x in pids:
        try:
            with open("/proc/" + x + "/cmdline", "r") as f:
                buf = " ".join(f.read().split(chr(0)));
                print(x + " " + buf);
        except IOError:
            pass

```

And use it like this: `python script.py >> /tmp/log`
