# PPII-TNFS-2022-GRP1

## Hiredis Installation

git clone https://github.com/redis/hiredis.git
sudo apt-get install -y libhiredis-dev
sudo mkdir /usr/local/include/hiredis
cd hiredis/
sudo mv -t /usr/local/include/hiredis hiredis.h alloc.h async.h read.h sds.h