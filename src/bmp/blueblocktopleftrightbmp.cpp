#include <nds.h>
#include "blueblocktopleftrightbmp.h"

static const u16 blueblocktopleftrightbmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 62662, 62662, 44032, 47203, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 62662, 
65205, 32768, 47203, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 62662, 65205, 65535, 56320, 32768, 47203, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 62662, 65205, 65535, 65205, 
62662, 44032, 32768, 44032, 47203, 32768, 32768, 32768, 32768, 32768, 32768, 
62662, 65205, 32768, 32768, 62662, 56320, 56320, 44032, 32768, 44032, 47203, 
32768, 32768, 32768, 32768, 32768, 65205, 32768, 48623, 48458, 32768, 56320, 
56320, 32768, 32768, 32768, 44032, 62662, 32768, 32768, 65205, 65205, 32768, 
65535, 32768, 65535, 65535, 32768, 32768, 48623, 48458, 32768, 32768, 47203, 
62662, 65205, 62662, 62662, 65205, 32768, 65535, 65535, 32768, 32768, 65535, 
65535, 32768, 65535, 32768, 44032, 47203, 62662, 47203, 56320, 62662, 65535, 
32768, 32768, 62662, 62662, 32768, 65535, 65535, 32768, 32768, 44032, 47203, 
47203, 32768, 44032, 56320, 65205, 65205, 62662, 62662, 62662, 32768, 32768, 
32768, 44032, 32768, 44032, 47203, 47203, 32768, 32768, 56320, 65205, 65205, 
62662, 62662, 56320, 47203, 32768, 32768, 32768, 32768, 44032, 47203, 32768, 
32768, 32768, 56320, 62662, 62662, 56320, 56320, 47203, 44032, 44032, 32768, 
32768, 44032, 47203, 32768, 32768, 32768, 32768, 32768, 56320, 47203, 47203, 
44032, 32768, 32768, 32768, 32768, 44032, 47203, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 56320, 47203, 47203, 47203, 47203, 47203, 47203, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

BlueBlockTopLeftRightBmp::BlueBlockTopLeftRightBmp() : WoopsiGfx::BitmapWrapper(blueblocktopleftrightbmp_Bitmap, 16, 16) { };