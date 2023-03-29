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

- git clone --recursive git@github.com:duanmeng/MacVelox.git
- cd MacVelox
- bash ./scripts/setup-mac-os.sh
- make [release | debug]

## Examples

### Hello Velox
```Shell
$ ./_build/release/src/example/mac_hello_velox
WARNING: Logging before InitGoogleLogging() is written to STDERR
I20230319 22:31:05.806710 3735465 HelloVelox.cpp:165] Hello Velox!
This is copied from example code of velox code base
https://github.com/facebookincubator/velox/blob/main/velox/examples/OpaqueType.cpp
I20230319 22:31:05.807632 3735465 HelloVelox.cpp:244] Executing simple opaque function:
I20230319 22:31:05.807869 3735465 HelloVelox.cpp:252] Found UserDefinedOutput: [0: zero]
I20230319 22:31:05.807879 3735465 HelloVelox.cpp:252] Found UserDefinedOutput: [1: one]
I20230319 22:31:05.807884 3735465 HelloVelox.cpp:252] Found UserDefinedOutput: [2: two]
I20230319 22:31:05.807888 3735465 HelloVelox.cpp:252] Found UserDefinedOutput: [3: three]
I20230319 22:31:05.807893 3735465 HelloVelox.cpp:252] Found UserDefinedOutput: [4: four]
I20230319 22:31:05.807898 3735465 HelloVelox.cpp:256] Executing vectorized opaque function:
I20230319 22:31:05.807924 3735465 HelloVelox.cpp:264] Found UserDefinedOutput: [0: zero]
I20230319 22:31:05.807929 3735465 HelloVelox.cpp:264] Found UserDefinedOutput: [1: one]
I20230319 22:31:05.807932 3735465 HelloVelox.cpp:264] Found UserDefinedOutput: [2: two]
I20230319 22:31:05.807936 3735465 HelloVelox.cpp:264] Found UserDefinedOutput: [3: three]
I20230319 22:31:05.807940 3735465 HelloVelox.cpp:264] Found UserDefinedOutput: [4: four]
I20230319 22:31:05.807945 3735465 HelloVelox.cpp:270] Executing vectorized opaque function (over constant opaque col):
I20230319 22:31:05.807967 3735465 HelloVelox.cpp:279] Found UserDefinedOutput: [0: zero]
I20230319 22:31:05.807972 3735465 HelloVelox.cpp:279] Found UserDefinedOutput: [1: one]
I20230319 22:31:05.807977 3735465 HelloVelox.cpp:279] Found UserDefinedOutput: [2: two]
I20230319 22:31:05.807982 3735465 HelloVelox.cpp:279] Found UserDefinedOutput: [3: three]
I20230319 22:31:05.807986 3735465 HelloVelox.cpp:279] Found UserDefinedOutput: [4: four]
I20230319 22:31:05.807991 3735465 HelloVelox.cpp:284] Number of instances of OpaqueState: 1
```

### Hello XSIMD
```Shell
$ ./_build/release/src/example/mac_hello_xsimd
Get simd size automatically.
Get simd info of int8_t
arch_type N5xsimd6neon64E
simd_type_size 16
register_type 16__simd128_int8_t
simd_register N5xsimd5types13simd_registerIaNS_6neon64EEE
Get simd info of int16_t
arch_type N5xsimd6neon64E
simd_type_size 8
register_type 17__simd128_int16_t
simd_register N5xsimd5types13simd_registerIsNS_6neon64EEE
Get simd info of int32_t
arch_type N5xsimd6neon64E
simd_type_size 4
register_type 17__simd128_int32_t
simd_register N5xsimd5types13simd_registerIiNS_6neon64EEE
Get simd info of int64_t
arch_type N5xsimd6neon64E
simd_type_size 2
register_type 17__simd128_int64_t
simd_register N5xsimd5types13simd_registerIxNS_6neon64EEE
Get simd info of double
arch_type N5xsimd6neon64E
simd_type_size 2
register_type 19__simd128_float64_t
simd_register N5xsimd5types13simd_registerIdNS_6neon64EEE
```

### Hello Arrow
```Shell
$ ./_build/release/src/example/mac_hello_arrow
Hello Arrow arrow::StatusCode::SerializationError string is Serialization error
Hello arrow/parquet magic number 3735928495 BlockSplitBloomFilter hash value is 1278943838006941606 MurmurHash3 value is 1278943838006941606
```

