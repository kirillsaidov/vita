#include <assert.h>
#include "../../inc/vita/core/core.h"

int32_t main(void) {
    // 4x5 matrix
    size_t nrows = 4, ncols = 5;
    size_t row = 1, col = 0;
    size_t idx = 5;

    // convert from 2d to 1d | (row, col) = (1, 0)
    assert(idx == index_2d_to_1d(row, col, ncols));

    // convert from 1d to 2d | idx = 5
    row = col = 999;
    index_1d_to_2d(&row, &col, idx, ncols);
    assert(row == 1); 
    assert(col == 0);

    return 0;
}
