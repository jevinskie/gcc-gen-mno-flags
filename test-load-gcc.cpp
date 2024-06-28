#undef NDEBUG
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <string>
#include <vector>

int main(int argc, const char **argv, const char **envp) {
    assert(argc >= 2);
    const auto gcc_handle = dlopen(argv[1], RTLD_NOW);
    printf("gcc_handle: %p\n", gcc_handle);
    if (!gcc_handle) {
        printf("dlopen errorno: %s error: %s\n", strerror(errno), dlerror());
        return 1;
    }
    void *fptr = dlsym(gcc_handle, "_Z23any_fp_register_operandP7rtx_def12machine_mode");
    if (!fptr) {
        printf("dlsym errorno: %s error: %s\n", strerror(errno), dlerror());
        return 1;
    }
    printf("fptr: %p\n", fptr);
    int (*main_fptr)(int argc, const char **argv, const char **envp) = nullptr;
    main_fptr = decltype(main_fptr)(dlsym(gcc_handle, "main"));
    if (!main_fptr) {
        printf("dlsym main errorno: %s error: %s\n", strerror(errno), dlerror());
        return 1;
    }
    argv[0] = "cc1";
    for (int i = 1; i < argc - 1; ++i) {
        argv[i] = argv[i + 1];
    }
    return main_fptr(argc - 1, argv, envp);
    dlclose(gcc_handle);
    return 0;
}
