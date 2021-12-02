#include "help.h"

int int_sqrt(int s) {
    int s0 = s / 2;
    if (s0) {
        int s1 = (s0 + s / s0) / 2;
        while (s1 < s0) {
            s0 = s1;
            s1= (s0 + s / s0) / 2;
        }
        return s0;
    } else {
        return s0;
    }
}
