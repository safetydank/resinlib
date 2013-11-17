#include "resin/math/Math.h"
#include "resin/thirdparty/tinymt/tinymt32.h"

#include <ctime>

using std::string;

namespace RESIN { namespace Math { 

static tinymt32_t* sMT = NULL;

double random()
{
    if (!sMT) {
        sMT = new tinymt32_t;
        tinymt32_init(sMT, int(::time(NULL)));
    }

    return tinymt32_generate_32double(sMT);
}

string generateUUID()
{
    // http://www.broofa.com/Tools/Math.uuid.htm
    char chars[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    char uuid[36];
    int rnd = 0, r;

    for ( int i = 0; i < 36; i ++ ) {
        if ( i == 8 || i == 13 || i == 18 || i == 23 ) {

            uuid[ i ] = '-';

        } else if ( i == 14 ) {

            uuid[ i ] = '4';

        } else {

            if (rnd <= 0x02) rnd = 0x2000000 + (random()*0x1000000);
            r = rnd & 0xf;
            rnd = rnd >> 4;
            uuid[i] = chars[(i == 19) ? (r & 0x3) | 0x8 : r];

        }
    }

    return string(uuid, uuid+36);
}

} }
