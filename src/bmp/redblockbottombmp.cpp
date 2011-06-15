#include <nds.h>
#include "redblockbottombmp.h"

static const u16 redblockbottombmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 39133, 32791, 35950, 35950, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 39133, 54975, 
54975, 39133, 32779, 35950, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 39133, 54975, 65535, 54975, 39133, 35950, 32779, 35950, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 39133, 54975, 48623, 32768, 39133, 
39133, 32768, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 32768, 32768, 
54975, 48623, 65535, 65535, 32768, 32768, 65535, 65535, 32768, 35950, 32768, 
32768, 32768, 32768, 32768, 39133, 65535, 32768, 65535, 32768, 32768, 32768, 
32768, 65535, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 39133, 65535, 
54975, 32768, 32768, 32791, 32791, 32768, 32768, 32768, 32779, 35950, 32768, 
32768, 32768, 32768, 39133, 65535, 39133, 32791, 32791, 32791, 32791, 35950, 
32779, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 39133, 54975, 32791, 
32791, 32791, 32791, 35950, 35950, 32779, 32768, 32779, 35950, 32768, 32768, 
32768, 32768, 39133, 39133, 32791, 32791, 35950, 35950, 32779, 32768, 32768, 
32779, 32779, 35950, 32768, 32768, 32768, 32768, 32768, 39133, 39133, 35950, 
32779, 32768, 32768, 32768, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 39133, 35950, 32779, 32768, 32768, 32779, 35950, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 39133, 
35950, 32779, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 54975, 39133, 35950, 32779, 35950, 32768, 32768, 
32768, 32768, 32768
};

RedBlockBottomBmp::RedBlockBottomBmp() : WoopsiGfx::BitmapWrapper(redblockbottombmp_Bitmap, 16, 16) { };