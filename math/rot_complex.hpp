#ifndef _GHP_MATH_ROT_COMPLEX_HPP_
#define _GHP_MATH_ROT_COMPLEX_HPP_

namespace ghp {

/**
  \brief simple complex number class for representing
    2D rotations in a compact and numerically stable 
    way.
  \tparam T - underlying floating point type
 */
template<typename T>
class rot_complex {
public:
  /** \brief create new rot_complex */
  inline rot_complex() {
    data_[0] = 1;
    data_[1] = 0;
  }
  /** \brief create new rot_complex 
    \param r - real part
    \param c - complex part */
  inline rot_complex(T r, T c) {
    data_[0] = r;
    data_[1] = c;
  }
  /** \brief copy constructor */
  inline rot_complex(const rot_complex &c) {
    for(int i=0; i<2; ++i) data_[i] = c.data_[i];
  }
  inline ~rot_complex() {
  }

  /** \brief element access */
  inline T& operator()(int i) { return data_[i]; }
  /** \brief element access */
  inline const T& operator()(int i) const { return data_[i]; }
  /** \brief element access */
  inline T& operator[](int i) { return (*this)(i); }
  /** \brief element access */
  inline const T& operator[](int i) const { return (*this)(i); }

  /** \brief element access */
  inline T& real() { return data_[0]; }
  /** \brief element access */
  inline const T& real() const { return data_[0]; }

  /** \brief element access */
  inline T& imag() { return data_[1]; }
  /** \brief element access */
  inline const T& imag() const { return data_[1]; }

  /** \brief arithmetic operation */
  template<typename T2>
  inline rot_complex operator*(const rot_complex<T2> &r) const {
    return rot_complex(
      real() * r.real() - imag() * r.imag(),
      imag() * r.real() + real() * r.imag()
    );
  }
  /** \brief arithmetic operation */
  template<typename T2>
  inline rot_complex operator/(const rot_complex<T2> &r) const {
    return (*this) * invert(r);
  }
  /** \brief arithmetic operation */
  template<typename T2>
  inline rot_complex& operator*=(const rot_complex<T2> &r) {
    rot_complex tmp = (*this) * r;
    (*this) = tmp;
    return *this;
  }
  /** \brief arithmetic operation */
  template<typename T2>
  inline rot_complex& operator/=(const rot_complex<T2> &r) {
    rot_complex tmp = (*this) / r;
    (*this) = tmp;
    return *this;
  }

  /** \brief return the inverse of this rotation */
  inline rot_complex invert() const {
    T denom = real() * real() + imag() * imag();
    return rot_complex(
      real() / denom,
      -imag() / denom
    );
  }
  /** \brief place the inverse of this rotation in
      the parameter.
    \param out - where to place the inverse of this
      rotation
   */
  template<typename T2>
  inline void invert(rot_complex<T2> &out) const {
    T denom = real()*real() + imag()*imag();
    out.real() = real() / denom;
    out.imag() = imag() / denom;
  }

  /** \brief ensure this remains a valid rotation */
  void normalize() {
    (*this) /= std::sqrt( real()*real() + imag()*imag() );
  }

private:
  T data_[2];
};

}

#endif

