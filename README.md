# PPII-TNFS-2022-GRP1

Loris ALEXANDRE | Sangoan BRIGUE | Hugo PAGNIEZ 
## Hiredis Installation

```
git clone https://github.com/redis/hiredis.git
sudo apt-get install -y libhiredis-dev
sudo mkdir /usr/local/include/hiredis
cd hiredis/
sudo mv -t /usr/local/include/hiredis hiredis.h alloc.h async.h read.h sds.h
```

## Build TNFS executables
```
sudo make server
sudo make tnfs
```

## Launch TNFS server
```
cd /exec
./tnfs-server
```

## Basic usage
```
./tnfs add <filename>
./tnfs get <CID>
./tnfs infos <CID>
./tnfs peer add <IP> <PORT>
./tnfs peer remove <IP> <PORT>
```
