#include <nds.h>
#include "orangeblockbounce1bmp.h"

static const u16 orangeblockbounce1bmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 33179, 33179, 33179, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 33179, 39551, 39551, 
33179, 33179, 36143, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 33179, 39551, 32768, 65535, 39551, 33179, 33179, 36143, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 57079, 57079, 52851, 32768, 
52851, 32768, 36143, 32973, 36143, 32768, 32768, 32768, 32768, 32768, 33179, 
32768, 65535, 32768, 65535, 57079, 57079, 52851, 32768, 32973, 36143, 32768, 
32768, 32768, 32768, 32768, 39551, 47871, 32768, 65535, 32768, 65535, 32768, 
65535, 32768, 32973, 36143, 32768, 32768, 32768, 32768, 33179, 47871, 39551, 
33179, 32768, 33179, 32768, 65535, 32768, 32768, 32973, 36143, 32768, 32768, 
32768, 32768, 33179, 33179, 36143, 36143, 32973, 32973, 32973, 32768, 32768, 
32973, 36143, 32768, 32768, 32768, 32768, 32768, 32768, 36143, 32973, 32973, 
32973, 32768, 32768, 32973, 36143, 36143, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 36143, 36143, 36143, 36143, 36143, 36143, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

OrangeBlockBounce1Bmp::OrangeBlockBounce1Bmp() : WoopsiGfx::BitmapWrapper(orangeblockbounce1bmp_Bitmap, 16, 16) { };