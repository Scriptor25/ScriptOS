.global __load_tr
.type __load_tr, @function
__load_tr:
    ltr %di
    ret
