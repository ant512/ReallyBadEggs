#include <nds.h>
#include "yellowblockexplodebmp.h"

static const u16 yellowblockexplodebmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 39869, 39869, 33527, 36302, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 39869, 39869, 55231, 
65535, 55231, 33527, 36302, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 39869, 39869, 48623, 32768, 55231, 39869, 48623, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 39869, 39869, 48623, 65535, 65535, 32768, 
48623, 65535, 65535, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 39869, 
48623, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 32768, 32768, 
32768, 32768, 32768, 32768, 39869, 32768, 65535, 65535, 32768, 65535, 65535, 
32768, 65535, 65535, 32768, 36302, 32768, 32768, 32768, 32768, 39869, 39869, 
32768, 65535, 65535, 32768, 32768, 65535, 65535, 32768, 32768, 36302, 32768, 
32768, 32768, 32768, 39869, 39869, 39869, 32768, 32768, 33527, 36302, 32768, 
32768, 32768, 32768, 36302, 32768, 32768, 32768, 32768, 39869, 33527, 33527, 
33527, 36302, 36302, 36302, 33131, 32768, 32768, 33131, 36302, 32768, 32768, 
32768, 32768, 32768, 36302, 36302, 33131, 33131, 33131, 33131, 32768, 32768, 
32768, 33131, 36302, 32768, 32768, 32768, 32768, 32768, 32768, 36302, 36302, 
33131, 32768, 32768, 32768, 32768, 33131, 36302, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 36302, 36302, 36302, 36302, 36302, 36302, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

YellowBlockExplodeBmp::YellowBlockExplodeBmp() : WoopsiGfx::BitmapWrapper(yellowblockexplodebmp_Bitmap, 16, 16) { };