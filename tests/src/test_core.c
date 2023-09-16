#include <assert.h>
#include "../../inc/vita/container/common.h"

int32_t main(void) {
    // 4x5 matrix
    size_t nrows = 4, ncols = 5, ndepth = 3;
    size_t row = 1, col = 0, depth = 1;
    size_t idx_2d = 5;
    size_t idx_3d = 25;

    // convert from 2d to 1d | (row, col) = (1, 0)
    assert(idx_2d == vt_index_2d_to_1d(row, col, ncols));

    // convert from 1d to 2d | idx_2d = 5
    row = col = 999;
    vt_index_1d_to_2d(&row, &col, idx_2d, ncols);
    assert(row == 1); 
    assert(col == 0);

    // convert 3d to 1d | (row, col, depth) = (1, 0, 1)
    assert(idx_3d == vt_index_3d_to_1d(row, col, depth, nrows, ncols));

    // convert from 3d to 1d | idx_3d = 25
    row = col = depth = 999;
    vt_index_1d_to_3d(&row, &col, &depth, idx_3d, nrows, ncols);
    assert(row == 1); 
    assert(col == 0);
    assert(depth == 1);
    
    return 0;
}
