# Notes

Check `file.psdb` content:

```bash
# remove qCompress header
dd if=file.psdb of=file.psdb.zlib bs=1 skip=4
# uncompress data
cat file.psdb.zlib | zlib-flate -uncompress > file.psdb.json
# display json
cat file.psdb.json | python3 -m json.tool
```
