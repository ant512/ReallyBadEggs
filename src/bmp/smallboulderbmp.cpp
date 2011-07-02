#include <nds.h>
#include "smallboulderbmp.h"

static const u16 smallboulderbmp_Bitmap[128
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
48623, 50809, 48623, 42281, 32904, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 48623, 65535, 57079, 52851, 37267, 32904, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 50809, 65535, 
65535, 57079, 48623, 42281, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 48623, 52851, 50809, 52851, 44395, 44395, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 42281, 48623, 44395, 
48623, 32970, 42281, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 42281, 42281, 42281, 42281, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768
};

SmallBoulderBmp::SmallBoulderBmp() : WoopsiGfx::BitmapWrapper(smallboulderbmp_Bitmap, 16, 8) { };