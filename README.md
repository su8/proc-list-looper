The sole purpose of this program is to log the pid name and cmdline in a loop, so when I kill some process to use `grep -i 'process name or pid' /tmp/log` to retrieve what I have killed.

The program is only useful if your init system is **not** systemd as they have journalctl.

By default it will log everything in **/tmp/log** every 1 second or until press ==CTRL + C==

# Usage

```bash
while true; do
  list
  sleep 1
  sort -u /tmp/log -o /tmp/log
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

# To install the module
sudo mkdir -p /lib/modules/`uname -r`/kernel/drivers/misc
sudo cp -r list.ko /lib/modules/`uname -r`/kernel/drivers/misc/list.ko
sudo depmod -a

sudo modprobe list

# To remove it from being loaded
sudo modprobe -r list
```

---

You can achieve something similar with python:

```python
import os;

pids = set(os.listdir("/proc"));
for x in pids:
    try:
        with open("/proc/" + x + "/cmdline", "r") as f:
            buf = " ".join(f.read().split(chr(0)));
            if not buf:
                with open("/proc/" + x + "/status", "r") as f2:
                    buf = "".join(f2.read().split()[1]);
            print(x + " " + buf);
    except IOError:
        pass;
```

And use it like this:
```bash
while true; do
  python script.py >> /tmp/log
  sleep 1
  sort -u /tmp/log -o /tmp/log
done
```
