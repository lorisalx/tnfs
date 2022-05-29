# PPII-TNFS-2022-GRP1

Loris ALEXANDRE | Sangoan BRIGUE | Hugo PAGNIEZ 

## Redis Installation

```
sudo apt-get install redis
```

## Hiredis Installation

```
cd src/lib/hiredis
make
make install
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
./tnfs peer add <IP>:<PORT>
./tnfs peer remove <IP>:<PORT>
```
