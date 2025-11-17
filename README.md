

# RV32I

A small RISC-V RV32 interpreter core


## ISA / Extension Support

The core targets the 32-bit RISC-V base integer ISA plus a few standard extensions.

Current configuration (as used by the e2e test programs and toolchain flags: `-march=rv32imf_zbb`):

- **RV32I** – base integer ISA
- **M** – integer multiply/divide
- *TODO:* **F** – single-precision floating-point
- *TODO:* **Zbb** – basic bit-manipulation instructions


## Prerequisites

* CMake **≥ 3.20**

* A C++20 compiler (GCC, Clang, or MSVC)

* A RISC-V cross [toolchain](https://github.com/riscv-collab/riscv-gnu-toolchain):

  ```bash
  riscv64-unknown-elf-gcc
  riscv64-unknown-elf-as
  ```

  must be on your `PATH`.

* GoogleTest installed and discoverable by CMake (either via a config package or system install).

---

## Build Types

All builds are standard CMake builds, the project uses a `ProjectOptions.cmake` module for sensible defaults:

* **Debug**

  * `-Og -g3 -fno-omit-frame-pointer`
  * Optional sanitizers (ASan, UBSan, TSan) for easier debugging
* **Release**

  * `-O3` and optional IPO/LTO

## Building (Debug)

From the project root:

```bash
cmake -S . -B build/debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DRV32I_ENABLE_TESTS=ON

cmake --build build/debug -j
```

This builds:

* `build/debug/rv32i`          – the interpreter CLI
* `build/debug/tests/rv32i_tests` – unit + e2e test binary
* `build/debug/tests/e2e_bins/*.rv32` – compiled RISC-V test programs for e2e tests

### Enabling sanitizers in Debug

For example, AddressSanitizer + UBSan:

```bash
cmake -S . -B build/debug-asan \
  -DCMAKE_BUILD_TYPE=Debug \
  -DRV32I_ENABLE_TESTS=ON \
  -DRV32I_ENABLE_ASAN=ON \
  -DRV32I_ENABLE_UBSAN=ON

cmake --build build/debug-asan -j
```

> Note: If your environment sets `LD_PRELOAD`, it can conflict with ASan. Consider building/running with `unset LD_PRELOAD` when using sanitizers.

---

## Building (Release)

```bash
cmake -S . -B build/release \
  -DCMAKE_BUILD_TYPE=Release

cmake --build build/release -j
```

This produces an optimized `rv32i` in `build/release/`.

You can also enable IPO/LTO (`RV32I_ENABLE_IPO=ON`, which is the default) for extra optimization.

## Running the Interpreter

After building (for example in `build/debug`):

```bash
./build/release/rv32i <path/to/program>
```

Loads a RISC-V ELF and runs it.

## Tests

### Enabling test build

Tests are gated by a top-level option:

```cmake
option(RV32I_ENABLE_TESTS "Build unit tests" OFF)
```

You must configure with:

```bash
cmake -S . -B build/debug \
  -DCMAKE_BUILD_TYPE=Debug \
  -DRV32I_ENABLE_TESTS=ON
```

### Running all tests

After building:

```bash
ctest --test-dir build/debug/tests
# or
./build/debug/tests/rv32i_tests
```

Both **unit tests** and **e2e tests** are in the same binary.

---

### Unit tests

Unit testing is focused on testing seperate instructions by encoding and dispatching them.
---

### End-to-End tests

The e2e tests work in two stages:

1. **Build RISC-V test programs** from C sources in `tests/e2e/*.c` using the cross toolchain and the common API:

   * `source/common/api.s`, `api.h`, `lib.h` provide:

     * `read`, `write`, `exit` syscall wrappers.
     * Small helpers for parsing integers, printing numbers, etc.
   * `tests/CMakeLists.txt` compiles each `.c` into a `.rv32` binary under `build/.../tests/e2e_bins/`.

2. **Run them through the interpreter** from C++ via GTest:

   * `tests/source/e2e_tests.cpp`:

     * Takes each `.rv32` binary.
     * Runs it via `rv32i` (e.g., `echo 'input' | rv32i program.rv32`).
     * Captures stdout and exit code.
     * Compares against expected results.

#### Example e2e programs

* `isqrt.c` – reads `n`, prints `floor(sqrt(n))`
* `factorial.c` – reads `n`, prints `n!`
* `bubblesort.c` – reads a count and list, prints the sorted list

Each uses the shared API:

```c
#include "api.h"
#include "lib.h"
```

#### Adding a new e2e program

1. Create `tests/e2e/<name>.c`, including `api.h` and `lib.h`.
2. Add `<name>` to the `E2E_PROGRAMS` list in `tests/CMakeLists.txt`.
3. Add an `E2ETestCase{...}` entry in `tests/source/e2e_tests.cpp` with the expected input/output.

Reconfigure + rebuild, and your new e2e program will be compiled and tested automatically.

