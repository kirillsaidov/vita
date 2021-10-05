#include "vita/container/mat.h"


mat_t *mat(const size_t rows, const size_t cols, const size_t elsize) {
	// allocate memory for mat_t
	mat_t *m = malloc(sizeof(mat_t));
	if(is_null(m)) {
		return NULL;
	}

	// mat_t init
	*m = (mat_t) {
		.ptr2 = malloc(rows * sizeof(void*)),
		.rows = rows,
		.cols = cols,
		.elsize = elsize,
	};

	if(is_null(m->ptr2)) {
		free(m);
		return NULL;
	}

	// allocate memory for rows*cols number of elements
	void* els = calloc(rows*cols, elsize);
	if(is_null(els)) {
		free(m->ptr2);
		free(m);
		return NULL;
	}

	// distribute the elements row-col-wise
	for(size_t i = 0; i < rows; i++) {
		m->ptr2[i] = els + i * cols * elsize;
	}

	return m;
}

mat_t *mat_dup(const mat_t *const m) {
	if(is_null(m)) {
		return NULL;
	}

	// create a new mat_t instance
	mat_t *mdup = mat(m->rows, m->cols, m->elsize);
	if(is_null(mdup)) {
		return NULL;
	}

	// copy values
	memcpy(*(mdup->ptr2), *(m->ptr2), m->rows * m->cols * m->elsize);

	return mdup;
}

void mat_free(mat_t *m) {
	if(is_null(m)) {
		return;
	}

	free(*(m->ptr2));
	free(m->ptr2);
	free(m);
}







size_t mat_rows(const mat_t *const m) {
	return m->rows;
}

size_t mat_cols(const mat_t *const m) {
	return m->cols;
}

size_t mat_size(const mat_t *const m) {
	return (m->rows * m->cols);
}







bool mat_clear(mat_t *const m) {
	if(is_null(m)) {
		return false;
	}

	// set values to 0
	memset(*m->ptr2, 0, m->rows * m->cols * m->elsize);

	return true;
}

bool mat_resize(mat_t *const m, const size_t rows, const size_t cols) {
	if(is_null(m)) {
		return false;
	}

	if(!rows || !cols) {
		vita_warn("cannot resize to 0!", __FUNCTION__);
		return false;
	}

	if(m->rows == rows && m->cols == cols) {
		return true;
	}

	// allocate memory for rows*cols number of elements
	void* els = realloc(*(m->ptr2), rows * cols * m->elsize);
	if(is_null(els)) {
		vita_warn("memory allocation failed!", __FUNCTION__);
		return false;
	}

	// distribute the elements row-col-wise
	for(size_t i = 0; i < rows; i++) {
		m->ptr2[i] = els + i * cols * m->elsize;
	}

	// update values
	m->rows = rows;
	m->cols = cols;

	return true;
}

bool mat_set(mat_t *const m, const void *val, const size_t atRow, const size_t atCol) {
	if(is_null(m) || is_null(val)) {
		return false;
	}

	if(!(atRow < m->rows) || !(atCol < m->cols)) {
		vita_warn("out-of-bounds access!", __FUNCTION__);
		return false;
	}

	// set the value
	memcpy(*m->ptr2 + (atRow * m->rows + atCol) * m->elsize, val, m->elsize);

	return true;
}

bool mat_seti32(mat_t *const m, const int val, const size_t atRow, const size_t atCol) {
	if(is_null(m)) {
		return false;
	}

	if(!(atRow < m->rows) || !(atCol < m->cols)) {
		vita_warn("out-of-bounds access!", __FUNCTION__);
		return false;
	}

	if(m->elsize != sizeof(val)) {
		vita_warn("expects <int32_t>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return mat_set(m, &val, atRow, atCol);
}

bool mat_seti64(mat_t *const m, const long val, const size_t atRow, const size_t atCol) {
	if(is_null(m)) {
		return false;
	}

	if(!(atRow < m->rows) || !(atCol < m->cols)) {
		vita_warn("out-of-bounds access!", __FUNCTION__);
		return false;
	}

	if(m->elsize != sizeof(val)) {
		vita_warn("expects <int64_t>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return mat_set(m, &val, atRow, atCol);
}

bool mat_setf(mat_t *const m, const float val, const size_t atRow, const size_t atCol) {
	if(is_null(m)) {
		return false;
	}

	if(!(atRow < m->rows) || !(atCol < m->cols)) {
		vita_warn("out-of-bounds access!", __FUNCTION__);
		return false;
	}

	if(m->elsize != sizeof(val)) {
		vita_warn("expects <float>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return mat_set(m, &val, atRow, atCol);
}

bool mat_setd(mat_t *const m, const double val, const size_t atRow, const size_t atCol) {
	if(is_null(m)) {
		return false;
	}

	if(!(atRow < m->rows) || !(atCol < m->cols)) {
		vita_warn("out-of-bounds access!", __FUNCTION__);
		return false;
	}

	if(m->elsize != sizeof(val)) {
		vita_warn("expects <double>! Aborting operation.", __FUNCTION__);
		return false;
	}

	return mat_set(m, &val, atRow, atCol);
}

void* mat_get(const mat_t *const m, const size_t atRow, const size_t atCol) {
	if(is_null(m)) {
		return NULL;
	}

	if(!(atRow < m->rows) || !(atCol < m->cols)) {
		vita_warn("out-of-bounds access!", __FUNCTION__);
		return NULL;
	}

	return (*m->ptr2 + (atRow * m->rows + atCol) * m->elsize);
}

int32_t mat_geti32(const mat_t *const m, const size_t atRow, const size_t atCol) {
	return *(int32_t*)(mat_get(m, atRow, atCol));
}

int64_t mat_geti64(const mat_t *const m, const size_t atRow, const size_t atCol) {
	return *(int64_t*)(mat_get(m, atRow, atCol));
}

float mat_getf(const mat_t *const m, const size_t atRow, const size_t atCol) {
	return *(float*)(mat_get(m, atRow, atCol));
}

double mat_getd(const mat_t *const m, const size_t atRow, const size_t atCol) {
	return *(double*)(mat_get(m, atRow, atCol));
}

void mat_foreach(const mat_t *const m, void (*func)(void*, size_t, size_t)) {
	if(is_null(m) || is_null(func)) {
		return;
	}

	for(size_t i = 0; i < m->rows; i++) {
		for(size_t j = 0; j < m->cols; j++) {
			func(mat_get(m, i, j), i, j);
		}
	}
}
















