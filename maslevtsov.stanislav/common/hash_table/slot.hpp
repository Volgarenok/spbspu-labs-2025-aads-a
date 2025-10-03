#ifndef HASH_TABLE_SLOT_HPP
#define HASH_TABLE_SLOT_HPP

namespace maslevtsov {
  namespace detail {
    enum class SlotState
    {
      EMPTY,
      OCCUPIED,
      DELETED,
    };

    template< typename T >
    struct Slot
    {
      T data;
      SlotState state;

      Slot():
        data(),
        state(SlotState::EMPTY)
      {}
    };
  }
}

#endif
