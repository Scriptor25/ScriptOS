#include <scriptos/paging.h>
#include <scriptos/pci.h>

pci::DeviceIterable::Iterator::Iterator(
    const u8* device_address,
    u8 function_index)
    : m_DeviceAddress(device_address),
      m_FunctionIndex(function_index)
{
}

bool pci::DeviceIterable::Iterator::operator==(const Iterator& iterator) const
{
    return m_DeviceAddress == iterator.m_DeviceAddress
        && m_FunctionIndex == iterator.m_FunctionIndex;
}

pair<
    u8,
    const pci::PCIHeader*>
pci::DeviceIterable::Iterator::operator*() const
{
    auto function_address = m_DeviceAddress + (static_cast<uptr>(m_FunctionIndex) << 12);
    paging::MapPage(function_address, function_address);

    auto function = reinterpret_cast<const PCIHeader*>(function_address);

    return { m_FunctionIndex, function };
}

pci::DeviceIterable::Iterator& pci::DeviceIterable::Iterator::operator++()
{
    m_FunctionIndex++;
    return *this;
}

pci::DeviceIterable::DeviceIterable(const u8* device_address)
    : m_DeviceAddress(device_address)
{
}

pci::DeviceIterable::Iterator pci::DeviceIterable::begin() const
{
    return { m_DeviceAddress, 0 };
}

pci::DeviceIterable::Iterator pci::DeviceIterable::end() const
{
    auto device = reinterpret_cast<const PCIHeader*>(m_DeviceAddress);
    paging::MapPage(device, device);

    if (device->HeaderType & 0x80)
        return { m_DeviceAddress, 8 };

    return { m_DeviceAddress, 1 };
}

pci::BusIterable::Iterator::Iterator(
    const u8* bus_address,
    u8 function_index)
    : m_BusAddress(bus_address),
      m_DeviceIndex(function_index)
{
}

bool pci::BusIterable::Iterator::operator==(const Iterator& iterator) const
{
    return m_BusAddress == iterator.m_BusAddress && m_DeviceIndex == iterator.m_DeviceIndex;
}

pair<
    u8,
    pci::DeviceIterable>
pci::BusIterable::Iterator::operator*() const
{
    auto device_address = m_BusAddress + (static_cast<uptr>(m_DeviceIndex) << 15);
    paging::MapPage(device_address, device_address);

    return { m_DeviceIndex, { device_address } };
}

pci::BusIterable::Iterator& pci::BusIterable::Iterator::operator++()
{
    m_DeviceIndex++;
    return *this;
}

pci::BusIterable::BusIterable(const u8* bus_address)
    : m_BusAddress(bus_address)
{
}

pci::BusIterable::Iterator pci::BusIterable::begin() const
{
    return { m_BusAddress, 0 };
}

pci::BusIterable::Iterator pci::BusIterable::end() const
{
    return { m_BusAddress, 32 };
}

pci::RootIterable::Iterator::Iterator(
    const u8* root_address,
    u8 bus_index)
    : m_RootAddress(root_address),
      m_BusIndex(bus_index)
{
}

bool pci::RootIterable::Iterator::operator==(const Iterator& iterator) const
{
    return m_RootAddress == iterator.m_RootAddress && m_BusIndex == iterator.m_BusIndex;
}

pair<
    u8,
    pci::BusIterable>
pci::RootIterable::Iterator::operator*() const
{
    auto bus_address = m_RootAddress + (static_cast<uptr>(m_BusIndex) << 20);
    paging::MapPage(bus_address, bus_address);

    return { m_BusIndex, { bus_address } };
}

pci::RootIterable::Iterator& pci::RootIterable::Iterator::operator++()
{
    m_BusIndex++;
    return *this;
}

pci::RootIterable::RootIterable(
    const u8* root_address,
    u8 start_bus,
    u8 end_bus)
    : m_RootAddress(root_address),
      m_StartBus(start_bus),
      m_EndBus(end_bus)
{
}

pci::RootIterable::Iterator pci::RootIterable::begin() const
{
    return { m_RootAddress, m_StartBus };
}

pci::RootIterable::Iterator pci::RootIterable::end() const
{
    return { m_RootAddress, m_EndBus };
}
