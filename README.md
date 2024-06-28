# gcc-gen-mno-flags
Parse GCC source to generate a list of required -mno-FEATURE flags like -mno-sse2 (to avoid any codegen that clobbers FPU/SIMD state) to enable building the kernel with -march=native.
