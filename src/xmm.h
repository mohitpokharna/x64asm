/*
Copyright 2013 eric schkufza

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef X64ASM_SRC_XMM_H
#define X64ASM_SRC_XMM_H

#include <iostream>

#include "src/operand.h"

namespace x64asm {

/** An XMM register. The 128-bit XMM registers are: XMM0 through XMM7; XMM8
    through XMM15 are available using REX.R in 64-bit mode.
*/
class Xmm : public Operand {
  // Needs access to constructor.
  friend class Constants;

  public:
    /** Copy constructor. */
    Xmm(const Xmm& rhs) : Operand(0,0) {
			val_ = rhs.val_;
		}
    /** Move constructor. */
    Xmm(Xmm&& rhs) {
			val_ = rhs.val_;
		}
    /** Copy assignment operator. */
    Xmm& operator=(const Xmm& rhs) {
			Xmm(rhs).swap(*this);
			return *this;
		}
    /** Move assignment operator. */
    Xmm& operator=(Xmm&& rhs) {
			Xmm(std::move(rhs)).swap(*this);
			return *this;
		}

    /** Returns true if this xmm register is well-formed. */
    constexpr bool check() {
  		return val_ < 16;
		}

    /** Comparison based on on val_. */
    constexpr bool operator<(const Xmm& rhs) {
			return val_ < rhs.val_;
		}
    /** Comparison based on on val_. */
    constexpr bool operator==(const Xmm& rhs) {
  		return val_ == rhs.val_;
		}
    /** Comparison based on on val_. */
    constexpr bool operator!=(const Xmm& rhs) {
			return !(*this == rhs);
		}

    /** Conversion based on underlying value. */
    constexpr operator uint64_t() {
			return val_;
		}

    /** STL-compliant hash. */
    constexpr size_t hash() {
			return val_;
		}
    /** STL-compliant swap. */
    void swap(Xmm& rhs) {
  		std::swap(val_, rhs.val_);
		}

    /** Returns the type of this operand */
    virtual Type type() const { return Type::XMM; }

		/** Reads this xmm register from an ostream using at&t syntax. */
		std::istream& read_att(std::istream& is);
    /** Writes this xmm register to an ostream using at&t syntax. */
    std::ostream& write_att(std::ostream& os) const;

  protected:
    /** Direct access to this constructor is disallowed. */
    constexpr Xmm(uint64_t val) : Operand(val) {}
};

/** The XMM register XMM0. */
class Xmm0 : public Xmm {
    // Needs access to constructor.
    friend class Constants;

  public:
    /** Returns true if this xmm register is %xmm0. */
    constexpr bool check() {
			return val_ == 0;
		}

    /** Returns the type of this operand */
    Type type() const { return Type::XMM_0; }

  private:
    /** Direct access to this constructor is disallowed. */
    constexpr Xmm0() : Xmm(0) {}
};

} // namespace x64asm

namespace std {

/** STL hash specialization. */
template <>
struct hash<x64asm::Xmm> {
  size_t operator()(const x64asm::Xmm& x) const {
		return x.hash();
	}
};

/** STL swap overload. */
inline void swap(x64asm::Xmm& lhs, x64asm::Xmm& rhs) {
	lhs.swap(rhs);
}

/** iostream overload. */
inline istream& operator>>(istream& is, x64asm::Xmm& x) {
	return x.read_att(is);
}
/** iostream overload. */
inline ostream& operator<<(ostream& os, const x64asm::Xmm& x) {
	return x.write_att(os);
}

} // namespace std

#endif
