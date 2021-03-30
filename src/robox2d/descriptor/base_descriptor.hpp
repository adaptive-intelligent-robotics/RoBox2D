#ifndef ROBOX2D_OFFICIAL_BASE_DESCRIPTOR_HPP
#define ROBOX2D_OFFICIAL_BASE_DESCRIPTOR_HPP

// for size_t
#include <cstddef>

namespace robox2d {
    class Simu;

    namespace descriptor {

        struct BaseDescriptor {
        public:
            BaseDescriptor(size_t desc_dump = 1);

            virtual ~BaseDescriptor() {}

            virtual void operator()() = 0;

            size_t desc_dump() const;

            void set_desc_dump(size_t desc_dump);

            void set_simu(Simu *simu) { _simu = simu; }

            const Simu *simu() const { return _simu; }

        protected:
            Simu *_simu;
            size_t _desc_period;
        };
    } // namespace descriptor
} // namespace robox2d

#endif //ROBOX2D_OFFICIAL_BASE_DESCRIPTOR_HPP
