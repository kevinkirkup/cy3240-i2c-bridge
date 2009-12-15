#include "ExceptionHandling.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

int main(void) {
    int count = 0;
    assert(++count == 1); /* should be reached. */
    try {
        assert(++count == 2); /* should be reached. */
        try {
            assert(++count == 3); /* should be reached. */
            try {
                assert(++count == 4); /* should be reached. */
                throw(1);
                assert(false); /* should NOT be reached. */
            } catch (2) {
                assert(false); /* should NOT be reached. */
            } endtry;
        } catch (1) {
            assert(++count == 5); /* should be reached. */
        } endtry;
        assert(++count == 6); /* should be reached. */
    } endtry;
    assert(++count == 7); /* should be reached. */
    return 0;
}
