#include <scriptos/efi.h>
#include <scriptos/multiboot2.h>
#include <scriptos/types.h>

void kernel_main(u32 magic, u8* info)
{
    if (magic != MULTIBOOT2_BOOTLOADER_MAGIC)
        return;

    (void) info;

    for (multiboot_tag* tag = (multiboot_tag*) (info + 8); tag->type != MULTIBOOT_TAG_TYPE_END;
         tag = (multiboot_tag*) ((u8*) tag + ((tag->size + 7) & ~7)))
    {
        if (tag->type == MULTIBOOT_TAG_TYPE_EFI64)
        {
            multiboot_tag_efi64* efi_tag = (multiboot_tag_efi64*) tag;
            EFI_SYSTEM_TABLE* efi_table = (EFI_SYSTEM_TABLE*) efi_tag->pointer;

            cstr vendor = (cstr) efi_table->FirmwareVendor;
            (void) vendor;

            void* runtime_services = efi_table->RuntimeServices;
            (void) runtime_services;
        }
    }

    for (;;)
        ;
}
