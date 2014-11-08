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

#include "src/reg_set.h"

#include <sstream>
#include <string>

#include "src/alias.h"
#include "src/constants.h"

using namespace std;

namespace x64asm {

RegSet& RegSet::operator+=(const M& rhs) {
	if (rhs.addr_or()) {
		if (rhs.contains_base()) {
			*this += Alias::to_double(rhs.get_base());
		}
		if (rhs.contains_index()) {
			*this += Alias::to_double(rhs.get_index());
		}
	} else {
		if (rhs.contains_base()) {
			*this += rhs.get_base();
		}
		if (rhs.contains_index()) {
			*this += rhs.get_index();
		}
	}
	return *this;
}

istream& RegSet::read_text(istream& is) {
	*this = RegSet::empty();
	string s;

	is >> s;
	if (s != "{") {
		is.setstate(ios::failbit);
		return is;
	}	

	while (true) {
		is >> s;
		if (s == "}") {
			break;
		}

    auto rh = ah;
    auto r8 = al;
    auto r16 = ax; 
    auto r32 = eax;
    auto r64 = rax;
    auto xmm = xmm0;
    auto ymm = ymm0;
		auto ef = eflags_cf;

		if (istringstream(s) >> r64) {
			(*this) += r64;
		}	else if (istringstream(s) >> r32) {
			(*this) += r32;
		}	else if (istringstream(s) >> r16) {
			(*this) += r16;
		}	else if (istringstream(s) >> r8) {
			(*this) += r8;
		}	else if (istringstream(s) >> rh) {
			(*this) += rh;
		}	else if (istringstream(s) >> ymm) {
			(*this) += ymm;
		}	else if (istringstream(s) >> xmm) {
			(*this) += xmm;
		}	else if (istringstream(s) >> ef) {
			(*this) += ef;
		} else {
			is.setstate(ios::failbit);
			return is;
		}
	}

	return is;
}

ostream& RegSet::write_text(ostream& os) const {
  os << "{";
	for (size_t i = 0; i < r64s.size(); ++i) {
		if (contains(r64s[i])) {
			os << " " << r64s[i];
		} else if (contains(r32s[i])) {
			os << " " << r32s[i];
		} else if (contains(r16s[i])) {
			os << " " << r16s[i];
		} else if (i < 4) {
			if (contains(rls[i])) {
				os << " " << rls[i];
			}
			if (contains(rhs[i])) {
				os << " " << rhs[i];
			}
		} else if (contains(rbs[i-4])) {
			os << " " << rbs[i-4];
		}
	}
	for (size_t i = 0; i < ymms.size(); ++i) {
		if (contains(ymms[i])) {
			os << " " << ymms[i];
		} else if (contains(xmms[i])) {
			os << " " << xmms[i];
		}
	}
	for (size_t i = 0; i < eflags.size(); i += eflags[i].width()) {
		if (contains(eflags[i])) {
			os << " " << eflags[i];
		}
	}
  os << " }";

	return os;
}

} // namespace x64asm
