#include <scriptos/paging.h>
#include <scriptos/pci.h>

kernel::PciDeviceIterable::Iterator::Iterator(
    const u8* device_address,
    u8 function_index)
    : m_DeviceAddress(device_address),
      m_FunctionIndex(function_index)
{
}

bool kernel::PciDeviceIterable::Iterator::operator==(const Iterator& iterator) const
{
    return m_DeviceAddress == iterator.m_DeviceAddress
        && m_FunctionIndex == iterator.m_FunctionIndex;
}

Pair<
    u8,
    const kernel::PciHeader*>
kernel::PciDeviceIterable::Iterator::operator*() const
{
    auto function_address = m_DeviceAddress + (static_cast<uptr>(m_FunctionIndex) << 12);
    kernel::MapPage(function_address, function_address);

    auto function = reinterpret_cast<const PciHeader*>(function_address);

    return { m_FunctionIndex, function };
}

kernel::PciDeviceIterable::Iterator& kernel::PciDeviceIterable::Iterator::operator++()
{
    m_FunctionIndex++;
    return *this;
}

kernel::PciDeviceIterable::PciDeviceIterable(const u8* device_address)
    : m_DeviceAddress(device_address)
{
}

kernel::PciDeviceIterable::Iterator kernel::PciDeviceIterable::begin() const
{
    return { m_DeviceAddress, 0 };
}

kernel::PciDeviceIterable::Iterator kernel::PciDeviceIterable::end() const
{
    auto device = reinterpret_cast<const PciHeader*>(m_DeviceAddress);
    kernel::MapPage(device, device);

    if (device->HeaderType & 0x80)
    {
        return { m_DeviceAddress, 8 };
    }

    return { m_DeviceAddress, 1 };
}

kernel::PciBusIterable::Iterator::Iterator(
    const u8* bus_address,
    u8 function_index)
    : m_BusAddress(bus_address),
      m_DeviceIndex(function_index)
{
}

bool kernel::PciBusIterable::Iterator::operator==(const Iterator& iterator) const
{
    return m_BusAddress == iterator.m_BusAddress && m_DeviceIndex == iterator.m_DeviceIndex;
}

Pair<
    u8,
    kernel::PciDeviceIterable>
kernel::PciBusIterable::Iterator::operator*() const
{
    auto device_address = m_BusAddress + (static_cast<uptr>(m_DeviceIndex) << 15);
    kernel::MapPage(device_address, device_address);

    return { m_DeviceIndex, { device_address } };
}

kernel::PciBusIterable::Iterator& kernel::PciBusIterable::Iterator::operator++()
{
    m_DeviceIndex++;
    return *this;
}

kernel::PciBusIterable::PciBusIterable(const u8* bus_address)
    : m_BusAddress(bus_address)
{
}

kernel::PciBusIterable::Iterator kernel::PciBusIterable::begin() const
{
    return { m_BusAddress, 0 };
}

kernel::PciBusIterable::Iterator kernel::PciBusIterable::end() const
{
    return { m_BusAddress, 32 };
}

kernel::PciIterable::Iterator::Iterator(
    const u8* root_address,
    u8 bus_index)
    : m_RootAddress(root_address),
      m_BusIndex(bus_index)
{
}

bool kernel::PciIterable::Iterator::operator==(const Iterator& iterator) const
{
    return m_RootAddress == iterator.m_RootAddress && m_BusIndex == iterator.m_BusIndex;
}

Pair<
    u8,
    kernel::PciBusIterable>
kernel::PciIterable::Iterator::operator*() const
{
    auto bus_address = m_RootAddress + (static_cast<uptr>(m_BusIndex) << 20);
    kernel::MapPage(bus_address, bus_address);

    return { m_BusIndex, { bus_address } };
}

kernel::PciIterable::Iterator& kernel::PciIterable::Iterator::operator++()
{
    m_BusIndex++;
    return *this;
}

kernel::PciIterable::PciIterable(
    const u8* root_address,
    u8 start_bus,
    u8 end_bus)
    : m_RootAddress(root_address),
      m_StartBus(start_bus),
      m_EndBus(end_bus)
{
}

kernel::PciIterable::Iterator kernel::PciIterable::begin() const
{
    return { m_RootAddress, m_StartBus };
}

kernel::PciIterable::Iterator kernel::PciIterable::end() const
{
    return { m_RootAddress, m_EndBus };
}
