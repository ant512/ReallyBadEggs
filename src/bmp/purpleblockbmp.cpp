#include <nds.h>
#include "purpleblockbmp.h"

static const u16 purpleblockbmp_Bitmap[256] __attribute__ ((aligned (4))) = {
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 62685, 56343, 47214, 47214, 47214, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 62685, 62685, 65215, 
62685, 56343, 47214, 44043, 47214, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 62685, 65215, 65535, 32768, 32768, 56343, 47214, 44043, 47214, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 62685, 65535, 32768, 65535, 65535, 
32768, 32768, 44043, 44043, 47214, 32768, 32768, 32768, 32768, 32768, 62685, 
65215, 32768, 65535, 32768, 65535, 65205, 32768, 32768, 32768, 47214, 32768, 
32768, 32768, 32768, 32768, 62685, 65215, 65215, 32768, 65535, 65205, 32768, 
44043, 44043, 32768, 47214, 32768, 32768, 32768, 32768, 32768, 62685, 62685, 
62685, 62685, 32768, 32768, 44043, 44043, 44043, 32768, 47214, 32768, 32768, 
32768, 32768, 32768, 32768, 56343, 56343, 56343, 56343, 44043, 44043, 44043, 
32768, 44043, 47214, 32768, 32768, 32768, 32768, 32768, 32768, 47214, 47214, 
47214, 47214, 47214, 44043, 32768, 44043, 47214, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 47214, 47214, 44043, 32768, 32768, 32768, 47214, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
47214, 44043, 44043, 32768, 44043, 44043, 47214, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 47214, 56343, 47214, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 32768, 
32768, 32768, 32768
};

PurpleBlockBmp::PurpleBlockBmp() : WoopsiGfx::BitmapWrapper(purpleblockbmp_Bitmap, 16, 16) { };