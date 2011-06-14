#include <nds.h>
#include "redblockleftbmp.h"

static const u16 redblockleftbmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 39133, 32791, 35950, 35950, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 39133, 54975, 
54975, 39133, 32779, 35950, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 39133, 54975, 65535, 54975, 39133, 35950, 32779, 35950, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 39133, 54975, 65535, 54975, 39133, 
39133, 32768, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 39133, 54975, 
65535, 48623, 32768, 39133, 32791, 32768, 65535, 65535, 32768, 35950, 32768, 
32768, 32768, 39133, 54975, 65535, 48623, 65535, 65535, 32768, 32791, 32768, 
32768, 65535, 32768, 32779, 35950, 32768, 32768, 39133, 39133, 39133, 32768, 
32768, 65535, 32768, 32791, 32791, 32768, 32768, 32768, 32779, 35950, 32768, 
32768, 32779, 35950, 32791, 39133, 32768, 32768, 32791, 32791, 32791, 35950, 
32779, 32768, 32779, 35950, 32768, 32768, 32768, 32768, 35950, 32791, 32791, 
32791, 32791, 32791, 35950, 35950, 32779, 32768, 32779, 35950, 32768, 32768, 
32768, 32768, 35950, 35950, 35950, 32791, 35950, 35950, 32779, 32768, 32768, 
32779, 32779, 35950, 32768, 32768, 32768, 32768, 32768, 35950, 35950, 35950, 
32779, 32768, 32768, 32779, 32779, 35950, 35950, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 35950, 35950, 35950, 35950, 35950, 35950, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

RedBlockLeftBmp::RedBlockLeftBmp() : WoopsiGfx::BitmapWrapper(redblockleftbmp_Bitmap, 16, 16) { };