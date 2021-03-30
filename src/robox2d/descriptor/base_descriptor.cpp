#include "base_descriptor.hpp"

#include "robox2d/descriptor/base_descriptor.hpp"
#include "robox2d/simu.hpp"

namespace robox2d {
    namespace descriptor {
        BaseDescriptor::BaseDescriptor(size_t desc_dump) : _desc_period(desc_dump) {}

        size_t BaseDescriptor::desc_dump() const
        {
          return _desc_period;
        }

        void BaseDescriptor::set_desc_dump(size_t desc_dump)
        {
          _desc_period = desc_dump;
        }
    } // namespace descriptor
} // namespace robox2d
