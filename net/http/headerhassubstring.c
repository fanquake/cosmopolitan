/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2021 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "libc/assert.h"
#include "libc/str/str.h"
#include "net/http/http.h"

/**
 * Returns true if standard header has substring.
 *
 * @param m is message parsed by ParseHttpRequest
 * @param b is buffer that ParseHttpRequest parsed
 * @param h is known header, e.g. kHttpAcceptEncoding
 * @param s should not contain comma
 * @param n is byte length of s where -1 implies strlen
 * @return true if substring present
 */
bool HeaderHasSubstring(struct HttpRequest *m, const char *b, int h,
                        const char *s, size_t n) {
  size_t i;
  assert(0 <= h && h < kHttpHeadersMax);
  if (n == -1) n = s ? strlen(s) : 0;
  if (m->headers[h].a) {
    if (memmem(b + m->headers[h].a, m->headers[h].b - m->headers[h].a, s, n)) {
      return true;
    }
    if (kHttpRepeatable[h]) {
      for (i = 0; i < m->xheaders.n; ++i) {
        if (GetHttpHeader(b + m->xheaders.p[i].k.a,
                          m->xheaders.p[i].k.b - m->xheaders.p[i].k.a) == h &&
            memmem(b + m->xheaders.p[i].v.a,
                   m->xheaders.p[i].v.b - m->xheaders.p[i].v.a, s, n)) {
          return true;
        }
      }
    }
  }
  return false;
}