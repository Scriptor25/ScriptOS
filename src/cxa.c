#define __ATEXIT_MAX_FUNCS 128

typedef struct
{
    void (*destructor)(void*);
    void* pointer;
    void* dso_handle;
} __atexit_func_entry;

static __atexit_func_entry __atexit_funcs[__ATEXIT_MAX_FUNCS];
static unsigned __atexit_func_count = 0;

void* __dso_handle = 0;

int __cxa_atexit(
    void (*destructor)(void*),
    void* pointer,
    void* dso_handle)
{
    if (__atexit_func_count >= __ATEXIT_MAX_FUNCS)
        return -1;

    __atexit_func_entry* entry = __atexit_funcs + __atexit_func_count++;
    entry->destructor = destructor;
    entry->pointer = pointer;
    entry->dso_handle = dso_handle;

    return 0;
}

void __cxa_finalize(void* f)
{
    unsigned i = __atexit_func_count;

    while (i--)
        if (!f || __atexit_funcs[i].destructor == f)
            __atexit_funcs[i].destructor(__atexit_funcs[i].pointer);
}
