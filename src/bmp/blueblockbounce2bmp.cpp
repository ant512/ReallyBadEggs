#include <nds.h>
#include "blueblockbounce2bmp.h"

static const u16 blueblockbounce2bmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 62662, 65205, 65535, 65205, 62662, 47203, 
44032, 44032, 47203, 32768, 32768, 32768, 32768, 32768, 32768, 62662, 65205, 
65535, 65205, 62662, 56320, 56320, 47203, 32768, 44032, 47203, 32768, 32768, 
32768, 32768, 62662, 65205, 32768, 48623, 48458, 32768, 62662, 32768, 48623, 
48458, 32768, 32768, 47203, 32768, 32768, 32768, 62662, 65205, 32768, 65535, 
32768, 32768, 62662, 32768, 65535, 32768, 32768, 32768, 44032, 47203, 32768, 
32768, 32768, 62662, 65205, 32768, 32768, 62662, 56320, 47203, 32768, 32768, 
44032, 32768, 44032, 47203, 32768, 32768, 32768, 62662, 62662, 62662, 56320, 
56320, 47203, 44032, 44032, 32768, 32768, 44032, 47203, 32768, 32768, 32768, 
32768, 32768, 56320, 47203, 47203, 44032, 32768, 32768, 32768, 32768, 44032, 
47203, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

BlueBlockBounce2Bmp::BlueBlockBounce2Bmp() : WoopsiGfx::BitmapWrapper(blueblockbounce2bmp_Bitmap, 16, 16) { };