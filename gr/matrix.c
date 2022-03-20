/* Matrices over generic rings */

#include "gr.h"

int
gr_mat_init(gr_mat_t mat, slong rows, slong cols, gr_ctx_t ctx)
{
    slong i, sz;

    sz = ctx->sizeof_elem;

    if (rows != 0)
        mat->rows = flint_malloc(rows * sizeof(gr_ptr));
    else
        mat->rows = NULL;

    if (rows != 0 && cols != 0)
    {
        mat->entries = (gr_ptr) flint_malloc(flint_mul_sizes(rows, cols) * sz);

        _gr_vec_init(mat->entries, rows * cols, ctx);

        for (i = 0; i < rows; i++)
            mat->rows[i] = GR_ENTRY(mat->entries, i * cols, sz);
    }
    else
    {
        mat->entries = NULL;
        for (i = 0; i < rows; i++)
            mat->rows[i] = NULL;
    }

    mat->r = rows;
    mat->c = cols;

    return GR_SUCCESS;
}

int
gr_mat_clear(gr_mat_t mat, gr_ctx_t ctx)
{
    if (mat->entries != NULL)
    {
        _gr_vec_clear(mat->entries, mat->r * mat->c, ctx);

        flint_free(mat->entries);
        flint_free(mat->rows);
    }
    else if (mat->r != 0)
    {
        flint_free(mat->rows);
    }

    return GR_SUCCESS;
}

int
gr_mat_swap(gr_mat_t mat1, gr_mat_t mat2, gr_ctx_t ctx)
{
    if (mat1 != mat2)
    {
        gr_mat_t tmp;
        *tmp = *mat1;
        *mat1 = *mat2;
        *mat2 = *tmp;
    }

    return GR_SUCCESS;
}

int
gr_mat_randtest(gr_mat_t mat, flint_rand_t state, void * options, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(mat, ctx);
    c = gr_mat_ncols(mat, ctx);

    status = GR_SUCCESS;
    for (i = 0; i < r; i++)
    {
        status |= _gr_vec_randtest(mat->rows[i], state, c, options, ctx);
    }

    return status;
}

int
gr_mat_is_zero(int * res, const gr_mat_t mat, gr_ctx_t ctx)
{
    int status, equal, this_status, this_equal;
    slong i, r, c;

    status = GR_SUCCESS;

    r = gr_mat_nrows(mat, ctx);
    c = gr_mat_ncols(mat, ctx);

    if (r == 0 || c == 0)
    {
        res[0] = 1;
        return GR_SUCCESS;
    }

    equal = 1;
    for (i = 0; i < r; i++)
    {
        this_status = _gr_vec_is_zero(&this_equal, mat->rows[i], c, ctx);

        if (this_status == GR_SUCCESS && !this_equal)
        {
            equal = 0;
            status = GR_SUCCESS;
            break;
        }
        else
        {
            status |= status;
        }
    }

    res[0] = equal;
    return status;
}

int
gr_mat_is_one(int * res, const gr_mat_t mat, gr_ctx_t ctx)
{
    int status, equal, this_status, this_equal;
    slong i, j, r, c, sz;

    status = GR_SUCCESS;

    r = gr_mat_nrows(mat, ctx);
    c = gr_mat_ncols(mat, ctx);

    if (r == 0 || c == 0)
    {
        res[0] = 1;
        return GR_SUCCESS;
    }

    sz = ctx->sizeof_elem;

    equal = 1;
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            if (i == j)
                this_status = gr_is_one(&this_equal, GR_MAT_ENTRY(mat, i, j, sz), ctx);
            else
                this_status = gr_is_zero(&this_equal, GR_MAT_ENTRY(mat, i, j, sz), ctx);

            if (this_status == GR_SUCCESS && !this_equal)
            {
                equal = 0;
                status = GR_SUCCESS;
                break;
            }
            else
            {
                status |= status;
            }
        }
    }

    res[0] = equal;
    return status;
}

int
gr_mat_is_neg_one(int * res, const gr_mat_t mat, gr_ctx_t ctx)
{
    int status, equal, this_status, this_equal;
    slong i, j, r, c, sz;

    status = GR_SUCCESS;

    r = gr_mat_nrows(mat, ctx);
    c = gr_mat_ncols(mat, ctx);

    if (r == 0 || c == 0)
    {
        res[0] = 1;
        return GR_SUCCESS;
    }

    sz = ctx->sizeof_elem;

    equal = 1;
    for (i = 0; i < r; i++)
    {
        for (j = 0; j < c; j++)
        {
            if (i == j)
                this_status = gr_is_neg_one(&this_equal, GR_MAT_ENTRY(mat, i, j, sz), ctx);
            else
                this_status = gr_is_zero(&this_equal, GR_MAT_ENTRY(mat, i, j, sz), ctx);

            if (this_status == GR_SUCCESS && !this_equal)
            {
                equal = 0;
                status = GR_SUCCESS;
                break;
            }
            else
            {
                status |= status;
            }
        }
    }

    res[0] = equal;
    return status;
}

int
gr_mat_equal(int * res, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    int status, equal, this_equal, this_status;
    slong i, r, c;

    status = GR_SUCCESS;

    r = gr_mat_nrows(mat1, ctx);
    c = gr_mat_ncols(mat1, ctx);

    if (r != gr_mat_nrows(mat2, ctx) ||
        c != gr_mat_ncols(mat2, ctx))
    {
        res[0] = 0;
        return GR_SUCCESS;
    }

    if (r == 0 || c == 0)
    {
        res[0] = 1;
        return GR_SUCCESS;
    }

    equal = 1;
    for (i = 0; i < r; i++)
    {
        this_status = _gr_vec_equal(&this_equal, mat1->rows[i], mat2->rows[i], c, ctx);

        if (this_status == GR_SUCCESS && !this_equal)
        {
            equal = 0;
            status = GR_SUCCESS;
            break;
        }
        else
        {
            status |= status;
        }
    }

    res[0] = equal;
    return status;
}

int
gr_mat_zero(gr_mat_t res, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);

    status = GR_SUCCESS;
    for (i = 0; i < r; i++)
    {
        status |= _gr_vec_zero(res->rows[i], c, ctx);
    }

    return status;
}

int
gr_mat_set_si(gr_mat_t res, slong v, gr_ctx_t ctx)
{
    int status;
    slong i, r, c, sz;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);
    sz = ctx->sizeof_elem;

    status = gr_mat_zero(res, ctx);

    if (r > 0 && c > 0)
    {
        status |= gr_set_si(GR_MAT_ENTRY(res, 0, 0, sz), v, ctx);

        for (i = 1; i < FLINT_MIN(r, c); i++)
            status |= gr_set(GR_MAT_ENTRY(res, i, i, sz),
                            GR_MAT_ENTRY(res, 0, 0, sz), ctx);
    }

    return status;
}

int
gr_mat_set_ui(gr_mat_t res, ulong v, gr_ctx_t ctx)
{
    int status;
    slong i, r, c, sz;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);
    sz = ctx->sizeof_elem;

    status = gr_mat_zero(res, ctx);

    if (r > 0 && c > 0)
    {
        status |= gr_set_ui(GR_MAT_ENTRY(res, 0, 0, sz), v, ctx);

        for (i = 1; i < FLINT_MIN(r, c); i++)
            status |= gr_set(GR_MAT_ENTRY(res, i, i, sz),
                            GR_MAT_ENTRY(res, 0, 0, sz), ctx);
    }

    return status;
}

int
gr_mat_set_fmpz(gr_mat_t res, const fmpz_t v, gr_ctx_t ctx)
{
    int status;
    slong i, r, c, sz;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);
    sz = ctx->sizeof_elem;

    status = gr_mat_zero(res, ctx);

    if (r > 0 && c > 0)
    {
        status |= gr_set_fmpz(GR_MAT_ENTRY(res, 0, 0, sz), v, ctx);

        for (i = 1; i < FLINT_MIN(r, c); i++)
            status |= gr_set(GR_MAT_ENTRY(res, i, i, sz),
                            GR_MAT_ENTRY(res, 0, 0, sz), ctx);
    }

    return status;
}

int
gr_mat_set_fmpq(gr_mat_t res, const fmpq_t v, gr_ctx_t ctx)
{
    int status;
    slong i, r, c, sz;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);
    sz = ctx->sizeof_elem;

    status = gr_mat_zero(res, ctx);

    if (r > 0 && c > 0)
    {
        status |= gr_set_fmpq(GR_MAT_ENTRY(res, 0, 0, sz), v, ctx);

        for (i = 1; i < FLINT_MIN(r, c); i++)
            status |= gr_set(GR_MAT_ENTRY(res, i, i, sz),
                            GR_MAT_ENTRY(res, 0, 0, sz), ctx);
    }

    return status;
}

int
gr_mat_one(gr_mat_t res, gr_ctx_t ctx)
{
    return gr_mat_set_si(res, 1, ctx);
}

int
gr_mat_set(gr_mat_t res, const gr_mat_t mat, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);

    if (r != gr_mat_nrows(mat, ctx) ||
        c != gr_mat_ncols(mat, ctx))
    {
        return GR_DOMAIN;
    }

    status = GR_SUCCESS;

    if (res != mat)
    {
        for (i = 0; i < r; i++)
        {
            status |= _gr_vec_set(res->rows[i], mat->rows[i], c, ctx);
        }
    }

    return status;
}

int
gr_mat_neg(gr_mat_t res, const gr_mat_t mat, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);

    if (r != gr_mat_nrows(mat, ctx) ||
        c != gr_mat_ncols(mat, ctx))
    {
        return GR_DOMAIN;
    }

    status = GR_SUCCESS;

    for (i = 0; i < r; i++)
    {
        status |= _gr_vec_neg(res->rows[i], mat->rows[i], c, ctx);
    }

    return status;
}

int
gr_mat_swap_entrywise(gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(mat1, ctx);
    c = gr_mat_ncols(mat1, ctx);

    if (r != gr_mat_nrows(mat2, ctx) ||
        c != gr_mat_ncols(mat2, ctx))
    {
        return GR_DOMAIN;
    }

    status = GR_SUCCESS;

    for (i = 0; i < r; i++)
    {
        status |= _gr_vec_swap(mat1->rows[i], mat2->rows[i], c, ctx);
    }

    return status;
}

int
gr_mat_add(gr_mat_t res, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);

    if (r != gr_mat_nrows(mat1, ctx) ||
        c != gr_mat_ncols(mat1, ctx) ||
        r != gr_mat_nrows(mat2, ctx) ||
        c != gr_mat_ncols(mat2, ctx))
    {
        return GR_DOMAIN;
    }

    status = GR_SUCCESS;
    for (i = 0; i < r; i++)
    {
        status |= _gr_vec_add(res->rows[i], mat1->rows[i], mat2->rows[i], c, ctx);
    }

    return status;
}

int
gr_mat_sub(gr_mat_t res, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    int status;
    slong i, r, c;

    r = gr_mat_nrows(res, ctx);
    c = gr_mat_ncols(res, ctx);

    if (r != gr_mat_nrows(mat1, ctx) ||
        c != gr_mat_ncols(mat1, ctx) ||
        r != gr_mat_nrows(mat2, ctx) ||
        c != gr_mat_ncols(mat2, ctx))
    {
        return GR_DOMAIN;
    }

    status = GR_SUCCESS;
    for (i = 0; i < r; i++)
    {
        status |= _gr_vec_sub(res->rows[i], mat1->rows[i], mat2->rows[i], c, ctx);
    }

    return status;
}

/* todo: use stream */
int
gr_mat_print(const gr_mat_t mat, gr_ctx_t ctx)
{
    int status;
    slong r, c;
    slong i, j;
    slong sz;

    sz = ctx->sizeof_elem;
    r = gr_mat_nrows(mat, ctx);
    c = gr_mat_ncols(mat, ctx);

    status = GR_SUCCESS;
    flint_printf("[");

    for (i = 0; i < r; i++)
    {
        flint_printf("[");

        for (j = 0; j < c; j++)
        {
            status |= gr_print(GR_MAT_ENTRY(mat, i, j, sz), ctx);
            if (j < c - 1)
                flint_printf(", ");
        }

        if (i < r - 1)
            flint_printf("],\n");
        else
            flint_printf("]");
    }
    flint_printf("]\n");
    return status;
}

int
gr_mat_mul_classical(gr_mat_t C, const gr_mat_t A, const gr_mat_t B, gr_ctx_t ctx)
{
    slong ar, ac, br, bc, i, j, sz;
    int status;

    ar = gr_mat_nrows(A, ctx);
    ac = gr_mat_ncols(A, ctx);
    br = gr_mat_nrows(B, ctx);
    bc = gr_mat_ncols(B, ctx);

    if (ac != br || ar != gr_mat_nrows(C, ctx) || bc != gr_mat_ncols(C, ctx))
        return GR_DOMAIN;

    if (br == 0)
    {
        return gr_mat_zero(C, ctx);
    }

    status = GR_SUCCESS;

    if (A == C || B == C)
    {
        gr_mat_t T;
        status |= gr_mat_init(T, ar, bc, ctx);
        status |= gr_mat_mul_classical(T, A, B, ctx);
        status |= gr_mat_swap_entrywise(T, C, ctx);
        status |= gr_mat_clear(T, ctx);
        return status;
    }

    sz = ctx->sizeof_elem;

    if (br == 1)
    {
        for (i = 0; i < ar; i++)
        {
            for (j = 0; j < bc; j++)
            {
                status |= gr_mul(GR_MAT_ENTRY(C, i, j, sz),
                                 GR_MAT_ENTRY(A, i, 0, sz),
                                 GR_MAT_ENTRY(B, 0, j, sz), ctx);
            }
        }
    }
    else
    {
        gr_ptr tmp;
        TMP_INIT;

        TMP_START;
        tmp = TMP_ALLOC(sz * br * bc);

        for (i = 0; i < br; i++)
            for (j = 0; j < bc; j++)
                memcpy(GR_ENTRY(tmp, j * br + i, sz), GR_MAT_ENTRY(B, i, j, sz), sz);

        for (i = 0; i < ar; i++)
        {
            for (j = 0; j < bc; j++)
            {
                status |= _gr_vec_dot(GR_MAT_ENTRY(C, i, j, sz), NULL, 0,
                    GR_MAT_ENTRY(A, i, 0, sz), GR_ENTRY(tmp, j * br, sz), br, ctx);
            }
        }

        TMP_END;
    }

    return status;
}

/*
static truth_t
ca_check_is_zero_fast(const ca_t x, ca_ctx_t ctx)
{
    if (CA_IS_QQ(x, ctx))
    {
        return fmpq_is_zero(CA_FMPQ(x)) ? T_TRUE : T_FALSE;
    }
    else
    {
        return T_UNKNOWN;
    }
}

static truth_t ca_check_is_zero_and_simplify(ca_t x, ca_ctx_t ctx)
{
    truth_t res = ca_check_is_zero_fast(x, ctx);

    if (res == T_UNKNOWN)
    {
        res = ca_check_is_zero(x, ctx);

        if (res == T_TRUE)
            ca_zero(x, ctx);
    }

    return res;
}
*/

int
gr_cmp_repr(gr_srcptr x, gr_srcptr y, gr_ctx_t ctx)
{
    return 0;
}

int
ca_mat_find_pivot(slong * pivot_row, gr_mat_t mat, slong start_row, slong end_row, slong column, gr_ctx_t ctx)
{
    slong best_row, i;
    int is_zero, status;
    int unknown;
    slong sz;

    if (end_row <= start_row)
        flint_abort();

/* todo: for ca-type rings */
#if 0
    /* First find the simplest element that is not trivially zero. With high probability
       this will actually be nonzero. */
    best_row = -1;

    for (i = start_row; i < end_row; i++)
    {
        is_zero = ca_check_is_zero_fast(ca_mat_entry(mat, i, column), ctx);

        if (is_zero != T_TRUE)
        {
            if (best_row == -1 || ca_cmp_repr(ca_mat_entry(mat, i, column), ca_mat_entry(mat, best_row, column), ctx) < 0)
            {
                best_row = i;
            }
        }
    }

    if (best_row != -1)
    {
        is_zero = ca_check_is_zero_and_simplify(ca_mat_entry(mat, best_row, column), ctx);

        if (is_zero == T_FALSE)
        {
            *pivot_row = best_row;
            return T_TRUE;
        }
    }
#endif

    /* If the above failed, go through all elements again and do more expensive checks.
       Todo: 1) support in-place simplifications.
             2) consider sorting above and traversing all entries in order of
                simplicity (not just the simplest element). */

    best_row = -1;
    unknown = 0;

    sz = ctx->sizeof_elem;

    for (i = start_row; i < end_row; i++)
    {
        status = gr_is_zero(&is_zero, GR_MAT_ENTRY(mat, i, column, sz), ctx);

        if (status == GR_SUCCESS)
        {
            if (!is_zero)
            {
                if (best_row == -1 || gr_cmp_repr(GR_MAT_ENTRY(mat, i, column, sz), GR_MAT_ENTRY(mat, best_row, column, sz), ctx) < 0)
                {
                    best_row = i;
                }
            }
        }
        else
        {
            unknown = 1;
        }
    }

    if (best_row == -1)
    {
        *pivot_row = -1;
        if (unknown)
            return GR_UNABLE;
        else
            return GR_DOMAIN;
    }
    else
    {
        *pivot_row = best_row;
        return GR_SUCCESS;
    }
}

int
gr_mat_is_empty(const gr_mat_t mat, gr_ctx_t ctx)
{
    return gr_mat_nrows(mat, ctx) == 0 || gr_mat_ncols(mat, ctx) == 0;
}

void
_gr_mat_swap_rows(gr_mat_t mat, slong * perm, slong r, slong s, gr_ctx_t ctx)
{
    if (r != s)
    {
        gr_ptr u;
        slong t;

        if (perm != NULL)
        {
            t = perm[s];
            perm[s] = perm[r];
            perm[r] = t;
        }

        u = mat->rows[s];
        mat->rows[s] = mat->rows[r];
        mat->rows[r] = u;
    }
}

int
gr_mat_lu_classical(slong * res_rank, slong * P, gr_mat_t LU, const gr_mat_t A, int full_rank_check, gr_ctx_t ctx)
{
    gr_ptr d, e;
    gr_ptr * a;
    slong i, j, m, n, r, rank, row, col, sz;
    int status;
    int pivot_status;
    GR_TMP_START;

    if (gr_mat_is_empty(A, ctx))
    {
        *res_rank = 0;
        return GR_SUCCESS;
    }

    GR_TMP_INIT2(d, e, ctx);

    m = gr_mat_nrows(A, ctx);
    n = gr_mat_ncols(A, ctx);
    sz = ctx->sizeof_elem;

    gr_mat_set(LU, A, ctx);

    a = LU->rows;

    rank = row = col = 0;
    for (i = 0; i < m; i++)
        P[i] = i;

    gr_init(d, ctx);
    gr_init(e, ctx);

    status = GR_SUCCESS;

    while (row < m && col < n)
    {
        pivot_status = ca_mat_find_pivot(&r, LU, row, m, col, ctx);

        /* We don't know whether there is a nonzero pivot element,
           so we can't determine the rank. */
        if (pivot_status == GR_UNABLE)
        {
            status = GR_UNABLE;
            break;
        }

        /* There is certainly no nonzero pivot element. */
        if (pivot_status == GR_DOMAIN)
        {
            /* We proved that the matrix is rank-deficient,
               accomplishing the goal. */
            if (full_rank_check)
            {
                status = GR_SUCCESS;
                rank = 0;
                break;
            }

            /* Continue with next column. */
            col++;
            continue;
        }

        rank++;

        if (r != row)
            _gr_mat_swap_rows(LU, P, row, r, ctx);

        /* Must be able to invert pivot element. */
        status |= gr_inv(d, GR_ENTRY(a[row], col, sz), ctx);
        if (status != GR_SUCCESS)
            break;

        for (j = row + 1; j < m; j++)
        {
            status |= gr_mul(e, GR_ENTRY(a[j], col, sz), d, ctx);
            status |= gr_neg(e, e, ctx);
            status |= _gr_vec_scalar_addmul(GR_ENTRY(a[j], col + 1, sz), GR_ENTRY(a[row], col + 1, sz), n - col - 1, e, ctx);
            status |= gr_zero(GR_ENTRY(a[j], col, sz), ctx);
            status |= gr_neg(GR_ENTRY(a[j], rank - 1, sz), e, ctx);
        }

        row++;
        col++;
    }

    GR_TMP_CLEAR2(d, e, ctx);
    GR_TMP_END;

    *res_rank = rank;
    return status;
}

GR_INLINE int
matrix_init(gr_mat_t res, gr_ctx_t ctx)
{
    return gr_mat_init(res, MATRIX_CTX(ctx)->n, MATRIX_CTX(ctx)->n, MATRIX_CTX(ctx)->base_ring);
}


int matrix_ctx_write(gr_stream_t out, gr_ctx_t ctx)
{
    slong n = MATRIX_CTX(ctx)->n;
    gr_ctx_ptr elem_ctx = MATRIX_CTX(ctx)->base_ring;

    gr_stream_write(out, "Ring of ");
    gr_stream_write_si(out, n);
    gr_stream_write(out, " x ");
    gr_stream_write_si(out, n);
    gr_stream_write(out, " matrices over ");
    gr_ctx_write(out, elem_ctx);
    return GR_SUCCESS;
}

int
matrix_ctx_clear(gr_ctx_t ctx)
{
    flint_free(ctx->elem_ctx);
    return GR_SUCCESS;
}

GR_INLINE int
matrix_clear(gr_mat_t res, gr_ctx_t ctx)
{
    return gr_mat_clear(res, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_swap(gr_mat_t mat1, gr_mat_t mat2, gr_ctx_t ctx)
{
    return gr_mat_swap(mat1, mat2, MATRIX_CTX(ctx)->base_ring);
}

/* TODO UNIFY */
GR_INLINE int
matrix_write(gr_stream_t out, gr_mat_t res, gr_ctx_t ctx)
{
    return gr_mat_print(res, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_randtest(gr_mat_t res, flint_rand_t state, void * options, gr_ctx_t ctx)
{
    return gr_mat_randtest(res, state, options, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_equal(int * equal, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    return gr_mat_equal(equal, mat1, mat2, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_set(gr_mat_t res, const gr_mat_t mat, gr_ctx_t ctx)
{
    return gr_mat_set(res, mat, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_set_si(gr_mat_t res, slong v, gr_ctx_t ctx)
{
    return gr_mat_set_si(res, v, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_set_ui(gr_mat_t res, ulong v, gr_ctx_t ctx)
{
    return gr_mat_set_ui(res, v, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_set_fmpz(gr_mat_t res, const fmpz_t v, gr_ctx_t ctx)
{
    return gr_mat_set_fmpz(res, v, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_set_fmpq(gr_mat_t res, const fmpq_t v, gr_ctx_t ctx)
{
    return gr_mat_set_fmpq(res, v, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_zero(gr_mat_t res, gr_ctx_t ctx)
{
    return gr_mat_zero(res, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_one(gr_mat_t res, gr_ctx_t ctx)
{
    return gr_mat_one(res, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_is_zero(int * res, const gr_mat_t mat, gr_ctx_t ctx)
{
    return gr_mat_is_zero(res, mat, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_is_one(int * res, const gr_mat_t mat, gr_ctx_t ctx)
{
    return gr_mat_is_one(res, mat, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_is_neg_one(int * res, const gr_mat_t mat, gr_ctx_t ctx)
{
    return gr_mat_is_neg_one(res, mat, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_neg(gr_mat_t res, const gr_mat_t mat, gr_ctx_t ctx)
{
    return gr_mat_neg(res, mat, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_add(gr_mat_t res, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    return gr_mat_add(res, mat1, mat2, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_sub(gr_mat_t res, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    return gr_mat_sub(res, mat1, mat2, MATRIX_CTX(ctx)->base_ring);
}

GR_INLINE int
matrix_mul(gr_mat_t res, const gr_mat_t mat1, const gr_mat_t mat2, gr_ctx_t ctx)
{
    return gr_mat_mul_classical(res, mat1, mat2, MATRIX_CTX(ctx)->base_ring);
}

/* todo: thread safe */
int _matrix_methods2_initialized = 0;
gr_static_method_table _matrix_static_table;
gr_method_tab_t _matrix_methods2;

gr_method_tab_input matrix_methods2[] =
{
    {GR_METHOD_CTX_WRITE,   (gr_funcptr) matrix_ctx_write},
    {GR_METHOD_CTX_CLEAR,   (gr_funcptr) matrix_ctx_clear},
    {GR_METHOD_INIT,        (gr_funcptr) matrix_init},
    {GR_METHOD_CLEAR,       (gr_funcptr) matrix_clear},
    {GR_METHOD_SWAP,        (gr_funcptr) matrix_swap},
    {GR_METHOD_RANDTEST,    (gr_funcptr) matrix_randtest},
    {GR_METHOD_WRITE,       (gr_funcptr) matrix_write},
    {GR_METHOD_ZERO,        (gr_funcptr) matrix_zero},
    {GR_METHOD_ONE,         (gr_funcptr) matrix_one},
    {GR_METHOD_IS_ZERO,     (gr_funcptr) matrix_is_zero},
    {GR_METHOD_IS_ONE,      (gr_funcptr) matrix_is_one},
    {GR_METHOD_IS_NEG_ONE,  (gr_funcptr) matrix_is_neg_one},
    {GR_METHOD_EQUAL,       (gr_funcptr) matrix_equal},
    {GR_METHOD_SET,         (gr_funcptr) matrix_set},
    {GR_METHOD_SET_UI,      (gr_funcptr) matrix_set_ui},
    {GR_METHOD_SET_SI,      (gr_funcptr) matrix_set_si},
    {GR_METHOD_SET_FMPZ,    (gr_funcptr) matrix_set_fmpz},
    {GR_METHOD_SET_FMPQ,    (gr_funcptr) matrix_set_fmpq},
    {GR_METHOD_NEG,         (gr_funcptr) matrix_neg},
    {GR_METHOD_ADD,         (gr_funcptr) matrix_add},
    {GR_METHOD_SUB,         (gr_funcptr) matrix_sub},
    {GR_METHOD_MUL,         (gr_funcptr) matrix_mul},
    {0,                     (gr_funcptr) NULL},
};

/* rename: gr_ctx_init_gr_mat */
void
gr_ctx_init_matrix(gr_ctx_t ctx, gr_ctx_t base_ring, slong n)
{
    ctx->flags = 0;

    if (base_ring->flags & GR_FINITE_RING)
        ctx->flags |= GR_FINITE_RING;

    ctx->sizeof_elem = sizeof(gr_mat_struct);
    ctx->elem_ctx = flint_malloc(sizeof(matrix_ctx_t));
    ctx->size_limit = WORD_MAX;

    ((matrix_ctx_t *) ctx->elem_ctx)->base_ring = (gr_ctx_struct *) base_ring;
    ((matrix_ctx_t *) ctx->elem_ctx)->n = n;

    if (!_matrix_methods2_initialized)
    {
        gr_method_tab_init_static(&_matrix_methods2, _matrix_static_table, matrix_methods2);
        _matrix_methods2_initialized = 1;
    }

    ctx->methods2 = &_matrix_methods2;
}
