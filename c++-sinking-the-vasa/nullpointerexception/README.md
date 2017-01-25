#bad idea #1504

*"let's out-ghetto win32's seh and add null pointer exception handling to c++ on unix"*

Scary enough, this is basically how the JVM does it.

```bash
$ clang++ -std=c++11 -g -o npe npe.cc
$ ./npe
first null
test1
second null
test2
third null
```
