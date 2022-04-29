# Introduction

A simple program written in `C` to show the usage of `Windows Socket`. 

# Build

```bash
mkdir build
cd ./src

gcc ws_server.c -lws2_32 -o ./build/ws_server.exe
gcc ws_client.c -lws2_32 -o ./build/ws_client.exe
```

# Run

```bash
"./build/ws_server.exe"
"./build/ws_client.exe"
```



