#/bin/bash
./build.sh
qemu-system-x86_64 -hda ./bin/os.bin
