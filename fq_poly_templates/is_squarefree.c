/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2013 Mike Hansen

******************************************************************************/


#ifdef T

#include "templates.h"

#include "ulong_extras.h"
int
_TEMPLATE(T, poly_is_squarefree)(const TEMPLATE(T, struct) * f, slong len, const TEMPLATE(T, ctx_t) ctx)
{
    TEMPLATE(T, struct) *fd, *g;
    TEMPLATE(T, t) invfd;
    slong dlen;
    int res;

    if (len <= 2)
        return len != 0;

    fd = _TEMPLATE(T, vec_init)(2 * (len - 1), ctx);
    g = fd + len - 1;

    _TEMPLATE(T, poly_derivative)(fd, f, len, ctx);
    dlen = len - 1;
    TEMPLATE(CAP_T, VEC_NORM)(fd, dlen, ctx);

    if (dlen)
    {
        TEMPLATE(T, init)(invfd, ctx);
        TEMPLATE(T, inv)(invfd, fd + (dlen - 1), ctx);
        res = (_TEMPLATE(T, poly_gcd)(g, f, len, fd, dlen, invfd, ctx) == 1);
        TEMPLATE(T, clear)(invfd, ctx);
    }
    else
        res = 0;                /* gcd(f, 0) = f, and len(f) > 2 */

    _TEMPLATE(T, vec_clear)(fd, 2 * (len - 1), ctx);
    return res;
}

int
TEMPLATE(T, poly_is_squarefree)(const TEMPLATE(T, poly_t) f, const TEMPLATE(T, ctx_t) ctx)
{
    return _TEMPLATE(T, poly_is_squarefree)(f->coeffs, f->length, ctx);
}


#endif
