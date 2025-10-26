#pragma once

#include "scriptos/stl.h"

#include <scriptos/types.h>

namespace pci
{
    cstr GetDeviceDescriptor(
        u8 base_class,
        u8 sub_class,
        u8 prog_if);
    cstr GetVendorName(u16 vendor_id);
    cstr GetDeviceName(
        u16 vendor_id,
        u16 device_id);

    struct PCIHeader
    {
        u16 VendorID;
        u16 DeviceID;

        u16 Command;
        u16 Status;

        u8 RevisionID;

        u8 ProgIF;
        u8 SubClass;
        u8 BaseClass;

        u8 CacheLineSize;
        u8 LatencyTimer;

        u8 HeaderType;

        u8 BIST;
    } __attribute__((packed));

    struct PCIDevice
    {
        PCIHeader Header;

        u32 BAR0;
        u32 BAR1;
        u32 BAR2;
        u32 BAR3;
        u32 BAR4;
        u32 BAR5;

        u32 CardBusCISPointer;

        u16 SubsystemVendorID;
        u16 SubsystemID;

        u32 ExpansionROMBaseAddress;

        u8 CapabilitiesPointer;

        u8 Reserved[7];

        u8 InterruptLine;
        u8 InterruptPIN;

        u8 MinGrant;
        u8 MaxLatency;
    } __attribute__((packed));

    struct PCI_To_PCI_Bridge
    {
        PCIHeader Header;

        // TODO
    } __attribute__((packed));

    struct PCI_To_CardBus_Bridge
    {
        PCIHeader Header;

        // TODO
    } __attribute__((packed));

    struct DeviceIterable
    {
    private:
        struct Iterator
        {
            Iterator(
                const u8* device_address,
                u8 function_index);

            bool operator==(const Iterator& iterator) const;
            pair<
                u8,
                const PCIHeader*>
            operator*() const;
            Iterator& operator++();

        private:
            const u8* m_DeviceAddress;
            u8 m_FunctionIndex;
        };

    public:
        DeviceIterable(const u8* device_address);

        Iterator begin() const;
        Iterator end() const;

    private:
        const u8* m_DeviceAddress;
    };

    struct BusIterable
    {
    private:
        struct Iterator
        {
            Iterator(
                const u8* bus_address,
                u8 device_index);

            bool operator==(const Iterator& iterator) const;
            pair<
                u8,
                DeviceIterable>
            operator*() const;
            Iterator& operator++();

        private:
            const u8* m_BusAddress;
            u8 m_DeviceIndex;
        };

    public:
        BusIterable(const u8* bus_address);

        Iterator begin() const;
        Iterator end() const;

        u8 Index;

    private:
        const u8* m_BusAddress;
    };

    struct RootIterable
    {
    private:
        struct Iterator
        {
            Iterator(
                const u8* root_address,
                u8 bus_index);

            bool operator==(const Iterator& iterator) const;
            pair<
                u8,
                BusIterable>
            operator*() const;
            Iterator& operator++();

        private:
            const u8* m_RootAddress;
            u8 m_BusIndex;
        };

    public:
        RootIterable(
            const u8* root_address,
            u8 start_bus,
            u8 end_bus);

        Iterator begin() const;
        Iterator end() const;

    private:
        const u8* m_RootAddress;
        u8 m_StartBus;
        u8 m_EndBus;
    };
}
