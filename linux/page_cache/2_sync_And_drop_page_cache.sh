# sync: let the file data write into the disk
free -m
sync
# 3 means: clean page_chche & slab
echo 3 > /proc/sys/vm/drop_caches
free -m
cat /proc/sys/vm/min_free_kbytes

