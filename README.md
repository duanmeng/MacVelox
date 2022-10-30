# MacVelox

## Prerequisites
*They may not be complete, install what you actually needed.*
- gflags
- glog
- libz
- openssl
- lz4
- zstd
- libsodium
- boost
- libevent
- snappy
- bz2
- xxhash

## Build Notes

- bash ./scripts/setup-macos.sh
- make [release | debug]

## Examples

### Hello Velox
```Shell
$ ./_build/release/example/mac_hello_velox
Hello Velox!
This is copied from example code of velox code base
https://github.com/facebookincubator/velox/blob/main/velox/examples/OpaqueType.cpp
WARNING: Logging before InitGoogleLogging() is written to STDERR
I20221030 15:28:32.737313 7982905 HelloVelox.cpp:280] Executing simple opaque function:
I20221030 15:28:32.738217 7982905 HelloVelox.cpp:288] Found UserDefinedOutput: [0: zero]
I20221030 15:28:32.738234 7982905 HelloVelox.cpp:288] Found UserDefinedOutput: [1: one]
I20221030 15:28:32.738245 7982905 HelloVelox.cpp:288] Found UserDefinedOutput: [2: two]
I20221030 15:28:32.738253 7982905 HelloVelox.cpp:288] Found UserDefinedOutput: [3: three]
I20221030 15:28:32.738262 7982905 HelloVelox.cpp:288] Found UserDefinedOutput: [4: four]
I20221030 15:28:32.738271 7982905 HelloVelox.cpp:292] Executing vectorized opaque function:
I20221030 15:28:32.738315 7982905 HelloVelox.cpp:300] Found UserDefinedOutput: [0: zero]
I20221030 15:28:32.738327 7982905 HelloVelox.cpp:300] Found UserDefinedOutput: [1: one]
I20221030 15:28:32.738337 7982905 HelloVelox.cpp:300] Found UserDefinedOutput: [2: two]
I20221030 15:28:32.738345 7982905 HelloVelox.cpp:300] Found UserDefinedOutput: [3: three]
I20221030 15:28:32.738354 7982905 HelloVelox.cpp:300] Found UserDefinedOutput: [4: four]
I20221030 15:28:32.738363 7982905 HelloVelox.cpp:306] Executing vectorized opaque function (over constant opaque col):
I20221030 15:28:32.738384 7982905 HelloVelox.cpp:315] Found UserDefinedOutput: [0: zero]
I20221030 15:28:32.738394 7982905 HelloVelox.cpp:315] Found UserDefinedOutput: [1: one]
I20221030 15:28:32.738401 7982905 HelloVelox.cpp:315] Found UserDefinedOutput: [2: two]
I20221030 15:28:32.738410 7982905 HelloVelox.cpp:315] Found UserDefinedOutput: [3: three]
I20221030 15:28:32.738418 7982905 HelloVelox.cpp:315] Found UserDefinedOutput: [4: four]
I20221030 15:28:32.738426 7982905 HelloVelox.cpp:320] Number of instances of OpaqueState: 1
```

### Hello Arrow
```Sheel
$ ./_build/release/example/mac_hello_arrow
Hello Arrow arrow::StatusCode::SerializationError string is Serialization error
Hello arrow/parquet magic number 3735928495 BlockSplitBloomFilter hash value is 1278943838006941606 MurmurHash3 value is 1278943838006941606
```

