#include <stdio.h>

#include "primap.h"

int main() {
    auto pm = pm_new();
    printf("%zu\n", pm_nth(&pm, 100'000'000));
    pm_delete(&pm);
}
