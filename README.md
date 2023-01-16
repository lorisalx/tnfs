# Telecom Nancy File System - TNFS

Loris ALEXANDRE | Sangoan BRIGUE | Hugo PAGNIEZ 

TNFS (Telecom Nancy File System) is a project developed by a group of three students at Telecom Nancy. It is an implementation of the InterPlanetary File System (IPFS) protocol, which aims to create a decentralized and distributed file system. TNFS utilizes peer-to-peer technology to enable users to share and access files without the need for a central server or point of control.

## Table of contents
* [Features](#features)
* [Installation](#installation)
* [Usage](#usage)
* [Contributing](#contributing)
* [License](#license)

## Features
All of the features discussed here are further detailed in the comprehensive [report](documents/TNFS_RapportFinal.pdf) submitted as part of our school project.
### Content addressing
We have implemented a customized version of CID (Content Identifier) to ensure the integrity of files. This version uses Base32 encoding and SHA-256 hashing algorithms, and is composed programmatically. The CID is made up of a prefix character indicating the encoding used, followed by hexadecimal characters representing the CID version, hash algorithm, hash size, and the hash itself.
### Bloc splitting
The system uses IPFS(InterPlanetary File System) and it has a feature called "block splitting" where large files are divided into multiple blocks of 256 kilo-bytes, allowing faster transit between peers and easy decentralization. Each block has a structure containing the file name, number of links, CID of links, data, and size. The parent block also contains the original file name and the list of CIDs of all the blocks containing the original file's data. This CID is what is transmitted to the user when adding a file to TNFS.

### Network exchanges
We have implemented a basic version of network exchanges using C programming language, as opposed to utilizing the more advanced Kademlia algorithm which is used in IPFS.

## Installation
### Redis Installation

```
sudo apt-get install redis
```

### Hiredis Installation

```
cd src/lib/hiredis
make
make install
```
## Usage
### Build TNFS executables
```
sudo make server
sudo make tnfs
```

### Launch TNFS server
```
cd /exec
./tnfs-server
```

### Basic usage
```
./tnfs add <filename>
./tnfs get <CID>
./tnfs infos <CID>
./tnfs peer add <IP>:<PORT>
./tnfs peer remove <IP>:<PORT>
```

## Contributing
Pull requests are welcome. Final goal is to translate Rust code to C. For major changes, please open an issue first
to discuss what you would like to change.

## License
[Licence](LICENSE)