#include <nds.h>
#include "greenblockleftrightbmp.h"

static const u16 greenblockleftrightbmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 33440, 45804, 33440, 33440, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 33440, 45804, 65535, 
45804, 45804, 33440, 33088, 32992, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 33440, 45804, 65535, 65535, 45804, 45804, 33440, 33216, 33088, 33088, 
32992, 32768, 32768, 33440, 33440, 33440, 45804, 65535, 48623, 32768, 33440, 
33440, 33216, 33088, 33088, 32992, 33216, 33440, 33440, 33216, 33216, 33440, 
65535, 48623, 48623, 65535, 32768, 33216, 33216, 32768, 32768, 32768, 33088, 
33216, 33216, 32992, 33088, 33440, 65535, 32768, 65535, 32768, 32768, 33216, 
32768, 65535, 48623, 32768, 32768, 32768, 32992, 32992, 32992, 33088, 33216, 
33440, 32768, 32768, 33088, 33088, 32768, 32768, 65535, 32768, 32768, 32992, 
32992, 33088, 33088, 32992, 33088, 33216, 33216, 33088, 32992, 32992, 32992, 
32768, 32768, 32768, 32992, 32992, 33088, 32768, 32768, 33088, 32992, 32992, 
33088, 32992, 32992, 32992, 32768, 32768, 32768, 32992, 32992, 33088, 32768, 
32768, 32768, 32768, 32768, 33088, 32992, 32992, 32768, 32768, 32768, 32768, 
32992, 32992, 33088, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 33088, 
33088, 32992, 32992, 32992, 32992, 32992, 33088, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 33088, 33088, 33088, 33088, 33088, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

GreenBlockLeftRightBmp::GreenBlockLeftRightBmp() : WoopsiGfx::BitmapWrapper(greenblockleftrightbmp_Bitmap, 16, 16) { };