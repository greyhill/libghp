#ifndef _GHP_UTIL_CHANGE_TRACKER_HPP_
#define _GHP_UTIL_CHANGE_TRACKER_HPP_

namespace ghp {

class change_tracker {
public:
  inline change_tracker()
      : changed_(false) { }
  inline ~change_tracker() { }

  inline const bool has_changed() const {
    return changed_;
  }

protected:
  inline void changed() {
    changed_ = true;
  }
  inline void unset_changed() {
    changed_ = false;
  }

private:
  bool changed_;
};

}

#endif

