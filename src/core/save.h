#ifndef SAVE_H
#define SAVE_H

#include "../models/promotion.h"

int test_save();

/* Save and load a Promotion structure in binary format.
 * save_promotion returns 0 on success, -1 on error.
 * load_promotion returns a pointer to a freshly allocated Promotion or NULL on error.
 */
int save_promotion(Promotion *prom, const char *filename);
Promotion *load_promotion(const char *filename);

#endif