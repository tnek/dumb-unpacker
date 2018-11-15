#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <stdint.h>

#define DEBUG

int __libc_start_main(int *(main) (int, char * *, char * *), 
        int argc, 
        char * * ubp_av, 
        void (*init) (void), 
        void (*fini) (void), 
        void (*rtld_fini) (void), 
        void (* stack_end)) {

    FILE *f = fopen("/proc/self/maps", "r");

    if (f == NULL) {
#ifdef DEBUG
        printf("Failed to load /proc/self/maps");
#endif
        exit(1);
    }

    int64_t text_addr;
    fscanf(f, "%lx", &text_addr);

#ifdef DEBUG
    printf("Binary .text loaded at %lx\n", text_addr);
#endif

    asm("int3");

    void *dls_handle;
    if ( !(dls_handle = dlopen("libc.so.6", RTLD_LAZY)) ) {
#ifdef DEBUG
        printf("Failed to load grab libc.so.6");
#endif
        exit(EXIT_FAILURE);
    }

    typeof(__libc_start_main) *old_libc_start_main;
    old_libc_start_main = dlsym(RTLD_NEXT, "__libc_start_main");

    return old_libc_start_main(main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}


