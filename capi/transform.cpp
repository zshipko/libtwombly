#include "twombly.h"

void draw_transform_matrix_scale(transform_matrix* mtx, double a, double b){
    (*(agg::trans_affine*)mtx->handle) *= agg::trans_affine_scaling(a, b);
}
void draw_transform_matrix_translate(transform_matrix* mtx, double a, double b){
    (*(agg::trans_affine*)mtx->handle) *= agg::trans_affine_translation(a, b);
}
void draw_transform_matrix_rotate(transform_matrix* mtx, double a){
    (*(agg::trans_affine*)mtx->handle) *= agg::trans_affine_rotation(a);
}

void draw_transform_matrix_reset(transform_matrix* mtx){
    ((agg::trans_affine*)mtx->handle)->reset();
}

transform_matrix* draw_get_transform_matrix(drawing* d){
    transform_matrix* mtx = (transform_matrix*)malloc(sizeof(transform_matrix));
    if (!mtx){
        return nullptr;
    }
    mtx->free_handle = 0;
    mtx->handle = (&((Drawing<rgb24>*)d->handle)->mtx);
    return mtx;
}

transform_matrix* draw_transform_matrix_create(){
    transform_matrix* mtx = (transform_matrix*)malloc(sizeof(transform_matrix));
    if (!mtx){
        return nullptr;
    }

    agg::trans_affine *_mtx = new agg::trans_affine();
    mtx->handle = _mtx;
    mtx->free_handle = 1;
    return mtx;
}

void draw_transform_matrix_free(transform_matrix *mtx){
    if (mtx->handle){
        if (mtx->free_handle){
            delete (agg::trans_affine*)mtx->handle;
            mtx->handle = nullptr;
        }
        free(mtx);
    }
}

double draw_transform_matrix_determinant(transform_matrix* mtx){
    return ((agg::trans_affine*)mtx->handle)->determinant();
}

void draw_transform_matrix_inverse_transform(transform_matrix* mtx, double *x, double *y){
    ((agg::trans_affine*)mtx->handle)->inverse_transform(x, y);
}

void draw_transform_matrix_transform(transform_matrix* mtx, double *x, double *y){
    ((agg::trans_affine*)mtx->handle)->transform(x, y);
}

void draw_transform_matrix_to_double(transform_matrix* mtx, double *d){
    ((agg::trans_affine*)mtx->handle)->store_to(d);
}

void draw_transform_matrix_from_double(transform_matrix* mtx, double *d){
    ((agg::trans_affine*)mtx->handle)->load_from(d);
}
