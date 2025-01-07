#include <scriptos/multiboot2.hpp>

MultibootInfo::Iter::Iter(const multiboot_tag *tag)
    : m_Tag(tag)
{
}

const multiboot_tag &MultibootInfo::Iter::operator*() const { return *m_Tag; }

MultibootInfo::Iter &MultibootInfo::Iter::operator++()
{
    m_Tag = (const multiboot_tag *)((u8 *)m_Tag + ((m_Tag->size + 7) & ~7));
    return *this;
}

MultibootInfo::Iter MultibootInfo::Iter::operator++(int)
{
    auto old = *this;
    ++(*this);
    return old;
}

bool MultibootInfo::Iter::operator==(const Iter &other) const { return other.m_Tag == m_Tag; }
bool MultibootInfo::Iter::operator!=(const Iter &other) const { return other.m_Tag != m_Tag; }

MultibootInfo::Iter MultibootInfo::begin() const
{
    return Iter((const multiboot_tag *)this + 1);
}

MultibootInfo::Iter MultibootInfo::end() const
{
    return Iter((const multiboot_tag *)((u8 *)this + m_TotalSize));
}
