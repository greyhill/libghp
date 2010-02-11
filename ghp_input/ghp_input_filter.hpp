#ifndef _GHP_INPUT_FILTER_HPP_
#define _GHP_INPUT_FILTER_HPP_

#include <list>

namespace ghp {
namespace input {

class filter_atom {
public:
  filter_atom() { }
  virtual ~filter_atom() { }
  virtual bool evaluate_now() const = 0;
};

template<typename T, bool (T::*F)() const>
class filter_atom_invoker0 : public filter_atom {
public:
  filter_atom_invoker0(const T &t) : t_(t) { }
  virtual ~filter_atom_invoker0() { }
  virtual bool evaluate_now() const { return t_.F(); }
private:
  const T &t_;
};

template<typename T, typename A, bool (T::*F)(A a) const>
class filter_atom_invoker1 : public filter_atom {
public:
  filter_atom_invoker1(const T &t, const A &a) : t_(t), a_(a) { }
  ~filter_atom_invoker1();
  virtual bool evaluate_now() const { return t_.F(a_); }
private:
  const T &t_;
  A a_;
};

class filter {
public:
  filter() { }
  filter(const filter_atom *a) {
    filter_atoms_.push_back(a);
  }
  filter(const filter &f) : filter_atoms_(f.filter_atoms_) { }
  ~filter() { }

  bool evaluate_now() const {
    for(auto it = filter_atoms_.begin(); it != filter_atoms_.end(); ++it) {
      if(!(*it)->evaluate_now()) return false;
    }
    return true;
  }

  filter operator+(const filter &f) const {
    filter newfilter;
    for(auto it = f.filter_atoms_.begin(); 
        it != f.filter_atoms_.end(); ++it) {
      newfilter.filter_atoms_.push_back(*it);
    }
    return newfilter;
  }

  filter& operator+=(const filter &f) {
    for(auto it = f.filter_atoms_.begin();
        it != f.filter_atoms_.end(); ++it) {
      filter_atoms_.push_back(*it);
    }
    return *this;
  }

  bool operator==(const filter &f) {
    return filter_atoms_ == f.filter_atoms_;
  }
private:
  std::list<const filter_atom*> filter_atoms_;
};

}}

#endif

