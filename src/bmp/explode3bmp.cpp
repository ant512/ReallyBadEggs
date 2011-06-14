#include <nds.h>
#include "explode3bmp.h"

static const u16 explode3bmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 48623, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 48623, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 48623, 32768, 32768, 32768, 48623, 57079, 48623, 32768, 32768, 32768, 
48623, 32768, 32768, 32768, 32768, 32768, 32768, 48623, 48623, 48623, 57079, 
65535, 57079, 48623, 48623, 48623, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 48623, 57079, 57079, 65535, 65535, 65535, 57079, 57079, 48623, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 48623, 57079, 65535, 32768, 32768, 
32768, 65535, 57079, 48623, 32768, 32768, 32768, 32768, 32768, 32768, 48623, 
57079, 65535, 32768, 32768, 32768, 32768, 32768, 65535, 57079, 48623, 32768, 
32768, 32768, 48623, 48623, 57079, 65535, 65535, 32768, 32768, 32768, 32768, 
32768, 65535, 65535, 57079, 48623, 48623, 32768, 32768, 32768, 48623, 57079, 
65535, 32768, 32768, 32768, 32768, 32768, 65535, 57079, 48623, 32768, 32768, 
32768, 32768, 32768, 32768, 48623, 57079, 65535, 32768, 32768, 32768, 65535, 
57079, 48623, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 48623, 57079, 
57079, 65535, 65535, 65535, 57079, 57079, 48623, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 48623, 48623, 48623, 57079, 65535, 57079, 48623, 48623, 
48623, 32768, 32768, 32768, 32768, 32768, 32768, 48623, 32768, 32768, 32768, 
48623, 57079, 48623, 32768, 32768, 32768, 48623, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 48623, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
48623, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

Explode3Bmp::Explode3Bmp() : WoopsiGfx::BitmapWrapper(explode3bmp_Bitmap, 16, 16) { };