#include <nds.h>
#include "blankblockbmp.h"

static const u16 blankblockbmp_Bitmap[256
] __attribute__ ((aligned (4))) = {
50809, 37267, 37267, 37267, 37267, 37267, 37267, 37267, 37267, 37267, 37267, 
37267, 37267, 37267, 37267, 32970, 37267, 37267, 33038, 33038, 33038, 33038, 
33038, 33038, 33038, 33038, 33038, 33038, 33038, 33038, 32970, 32838, 37267, 
33038, 32970, 32970, 32970, 32970, 32970, 32970, 33038, 33038, 32970, 32970, 
32970, 32970, 32904, 32838, 37267, 33038, 32970, 32904, 32904, 32904, 32970, 
33038, 37267, 32970, 32970, 32904, 32904, 32970, 32838, 32838, 37267, 33038, 
32904, 32838, 32904, 32970, 33038, 37267, 32970, 32904, 32904, 32970, 32970, 
33038, 32838, 32803, 37267, 33038, 32904, 32970, 32970, 33038, 37267, 33038, 
32970, 32838, 32970, 33038, 37267, 33038, 32838, 32803, 37267, 33038, 32970, 
32970, 33038, 33038, 37267, 33038, 32904, 32838, 33038, 33038, 37267, 32970, 
32904, 32803, 37267, 33038, 32970, 33038, 37267, 33038, 33038, 32838, 32838, 
33038, 37267, 37267, 33038, 32970, 32904, 32838, 37267, 33038, 33038, 33038, 
37267, 32904, 32838, 32838, 32970, 33038, 33038, 33038, 32970, 32970, 32838, 
32838, 37267, 33038, 32970, 33038, 32970, 32904, 32970, 32970, 33038, 37267, 
32970, 32970, 32970, 32904, 32838, 32803, 37267, 33038, 32970, 32970, 32838, 
32904, 32970, 33038, 37267, 33038, 33038, 32970, 32838, 32904, 32838, 32838, 
37267, 33038, 32970, 32904, 32904, 32970, 37267, 37267, 33038, 33038, 32970, 
32904, 32904, 32904, 32904, 32838, 37267, 33038, 32970, 32970, 32970, 33038, 
37267, 37267, 32970, 32970, 32838, 32838, 32904, 32970, 32904, 32838, 37267, 
33038, 32970, 32970, 32970, 33038, 32970, 33038, 32970, 32970, 32838, 32970, 
32970, 32970, 32904, 32838, 37267, 32970, 32904, 32904, 32904, 32904, 32904, 
32838, 32803, 32838, 32904, 32904, 32904, 32904, 32838, 32838, 32970, 32838, 
32838, 32838, 32838, 32803, 32803, 32803, 32803, 32803, 32838, 32838, 32838, 
32838, 32838, 32768
};

BlankBlockBmp::BlankBlockBmp() : WoopsiGfx::BitmapWrapper(blankblockbmp_Bitmap, 16, 16) { };