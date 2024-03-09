/*
This file is part of Nameplate.
Copyright (C) 2024 BunnyBox Productions

Nameplate is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

Nameplate is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with Nameplate.
If not, see <https://www.gnu.org/licenses/>.
*/

module;

#include <windows.h>
#include <psapi.h>

#include <array>
#include <tuple>

#include <iostream>
#include <stddef.h>
#include <inttypes.h>

export module findsignature;

// should be moved elsewhere or removed
// module name should not be hardcoded
class Haystack {
    public:
        constexpr Haystack() : haystack(nullptr), haystacklen(0) {}

        void Refresh() {
            MODULEINFO ModuleInfo;
            ZeroMemory(&ModuleInfo, sizeof(ModuleInfo));
            GetModuleInformation(GetCurrentProcess(), GetModuleHandle("FFXiMain.dll"), &ModuleInfo, sizeof(ModuleInfo));

            haystack = (const uint8_t*) ModuleInfo.lpBaseOfDll;
            haystacklen = (size_t) ModuleInfo.SizeOfImage;
        }

        void Retrieve(const uint8_t** pHaystack, size_t* pHaystackLen) {
            if (haystack == nullptr) {
                Refresh();
            }

            if (pHaystack != nullptr) {
                *pHaystack = haystack;
            }

            if (pHaystackLen != nullptr) {
                *pHaystackLen = haystacklen;
            }
        }
    private:
        const uint8_t* haystack;
        size_t haystacklen;
};

// should be moved elsewhere or removed
static Haystack gHaystack;

template<size_t N>
struct SLit {
    constexpr SLit(const char (&str)[N]) {
        std::copy_n(str, N, value);
    }

    uint8_t value[N];
};

static constexpr uint8_t digit(uint8_t x) {
    if (x >= 'A' && x <= 'F') {
        return 10 + (x - 'A');
    } else {
        return x - '0';
    }
}

template<SLit slit>
static consteval auto innerrle() {
    // oh no!
    // warning - does not have error checking for:
    // - 129+ byte runs
    // - 256+ byte outputs
    // - invalid hex
    // - lowercase hex
    // - trailing extra character

    // slit - string literal in a wrapper
    // soff - offset into the string literal; the recursion variable
    // aoff - position into the output buffer
    // hedr - position of the current rle header byte
    // phase - keeps track of wildcard vs. literal state
    // hrun - the current literal run length
    // hbgn - position of the current best literal run
    // hbst - length of the best literal run

    // first output byte:
    // - position of largest exact match

    // header bytes:
    // - <0x80: n+1 wildcard bytes
    // - >0x7F: (n-0x7F) exact match bytes

    // could be cleaned up quite a bit now that it is a consteval
    // function and not a recursive template

    enum PHASE {
        INITIAL,
        WILDCARD,
        LITERAL,
    };

    constexpr size_t slen = sizeof(slit.value);
    size_t soff = 0;
    size_t aoff = 1;
    size_t hedr = 0;
    PHASE phase = PHASE::INITIAL;
    size_t hrun = 0;
    size_t hbgn = 0;
    size_t hbst = 0;

    // Preallocate worst case.
    std::array<uint8_t, slen+1> a;
    a.fill(0);

    for (soff = 0; soff < (slen&(~1)); soff += 2) {
        uint8_t s0 = slit.value[soff+0];
        uint8_t s1 = slit.value[soff+1];

        if (s0 == '?' && s1 == '?') {
            // Wildcard phase
            if (phase != PHASE::WILDCARD) {
                a[aoff] = 0;
                hedr = aoff;
                aoff = aoff + 1;
                phase = PHASE::WILDCARD;
                hrun = 0;
            } else {
                a[hedr] = a[hedr] + 1;
                phase = PHASE::WILDCARD;
                hrun = 0;
            }
        } else {
            // Literal phase
            uint8_t t = digit(s0);
            uint8_t o = digit(s1);
            uint8_t v = (t * 16) + o;

            if (phase != PHASE::LITERAL) {
                a[aoff+0] = 0x80;
                a[aoff+1] = v;
                hedr = aoff;
                phase = PHASE::LITERAL;
                hrun = 1;
                aoff = aoff + 2;
            } else {
                a[hedr] = 0x80 | ((a[hedr] & 0x7F) + 1);
                a[aoff] = v;

                if (hrun > hbst) {
                    phase = PHASE::LITERAL;
                    hrun = hrun + 1;
                    hbgn = hedr;
                    hbst = hrun;
                    aoff = aoff + 1;
                } else {
                    phase = PHASE::LITERAL;
                    hrun = hrun + 1;
                    aoff = aoff + 1;
                }
            }
        }
    }

    // store the starting position in the start.
    // resulting scan algo:
    // jump to hbgn
    // scan right to end
    // jump to start
    // scan right to hbgn
    a[0] = static_cast<uint8_t>(hbgn);
    return std::tuple<std::array<uint8_t, slen+1>, size_t>(a, aoff);
}

template<SLit slit>
static consteval auto rleize() {
    constexpr auto t = innerrle<slit>();

    constexpr auto r = std::get<0>(t);
    constexpr auto l = std::get<1>(t);

    // Truncate result.
    std::array<uint8_t, l> tmp;
    std::copy_n(r.begin(), l, tmp.begin());

    return tmp;
}

static const uint8_t* GetNeedle(const uint8_t* sig, size_t length) {
    (void) length;
    return sig + sig[0];
}

static bool IsSigMatch(const uint8_t* haystack, const uint8_t* start, const uint8_t* end, const uint8_t* entry, size_t rewind) {
    const uint8_t* p = entry;
    const uint8_t* q = haystack;

    while (p < end) {
        uint8_t header = p[0];
        uint8_t runlen = (header & 0x7F) + 1;

        p += 1;

        if (header >= 0x80) {
            if (memcmp(p, q, runlen) != 0) {
                return false;
            }
            p += runlen;
        }

        q += runlen;
    }

    p = start;
    q = haystack - rewind;

    while (p < entry) {
        uint8_t header = p[0];
        uint8_t runlen = (header & 0x7F) + 1;

        p += 1;

        if (header >= 0x80) {
            if (memcmp(p, q, runlen) != 0) {
                return false;
            }
            p += runlen;
        }

        q += runlen;
    }

    return true;
}

export uint8_t* FindSig(const uint8_t* sig, size_t length) {
    const uint8_t* haystack;
    size_t haystacklen;

    // should be moved elsewhere or removed
    // (caller should provide these values)
    gHaystack.Retrieve(&haystack, &haystacklen);

    const uint8_t* needle = GetNeedle(sig, length);
    if (needle == nullptr) {
        return nullptr;
    }

    size_t needlelen = (needle[0] & 0x7F) + 1;

    const uint8_t* start = sig + 1;
    const uint8_t* end = sig + length;
    const uint8_t* entry = needle;
    needle++;

    // because it isn't cached...
    size_t rewind = 0;

    const uint8_t* p = start;
    while (p < entry) {
        uint8_t header = p[0];
        uint8_t runlen = (header & 0x7F) + 1;

        p += 1;

        if (header >= 0x80) {
            p += runlen;
        }

        rewind += runlen;
    }

    if (needlelen > 1) {
        const uint8_t* x = needle;
        size_t m = needlelen;

        const uint8_t* y = haystack;
        size_t n = haystacklen;

        size_t j, k, ell;

        if (x[0] == x[1]) {
            k = 2;
            ell = 1;
        } else {
            k = 1;
            ell = 2;
        }

        j = 0;
        while (j <= n - m) {
            if (x[1] != y[j + 1]) {
                 j += k;
            } else {
                if (IsSigMatch(&y[j], start, end, entry, rewind)) {
                    return (uint8_t*) &y[j - rewind];
                }

                j += ell;
            }
        }
    } else {
        // edge case I don't care to support right now.
        // I would much rather find two bytes that I can match on.
        // // todo - need to duplicate the above loop into this code path
        // return (const uint8_t*) memchr((const void*) haystack, needle[0], haystacklen);
    }

    return nullptr;
}

template <size_t N>
class Signature {
    public:
        constexpr Signature(std::array<uint8_t, N> sig) : signature(sig) {}

        uint8_t* Find() const {
            return FindSig(signature.data(), signature.size());
        }

    private:
        std::array<uint8_t, N> signature;
};

export template<SLit slit>
consteval auto MakeSig() {
    constexpr auto n = rleize<slit>();
    return Signature<n.size()>(n);
}
