/*
 *  Example: Moving average
 */

#include "gmv_filter.h"
#include <stdio.h>

using namespace gcl;


int main()
{
    MVFilter mvf(5);

    // printf("%d\n", static_cast<int>(22.56));

    for (int i=1; i<=20; i++) {
        double r = mvf.filter(i * 1.3);
        printf("r=%lf\n", r);
    }
    return 0;
}
