#include <iostream>
#include "cstdio"

#define NUM_TRIALS	10
#define NUMS 300 // number of subdivisions on the shape
#define NUMT 4 // number of threads used to calculate


#define XMIN	 0.
#define XMAX	 3.
#define YMIN	 0.
#define YMAX	 3.

#define Z00	0.
#define Z10	1.
#define Z20	0.
#define Z30	0.

#define Z01	1.
#define Z11	6.
#define Z21	1.
#define Z31	0.

#define Z02	0.
#define Z12	1.
#define Z22	0.
#define Z32	4.

#define Z03	3.
#define Z13	2.
#define Z23	3.
#define Z33	3.

float Height( int, int );

float Height( int iu, int iv )	{ // iu,iv = 0 .. NUMS-1 
    float u = (float)iu / (float)(NUMS-1);
    float v = (float)iv / (float)(NUMS-1);

    // the basis functions:
    float bu0 = (1.-u) * (1.-u) * (1.-u);
    float bu1 = 3. * u * (1.-u) * (1.-u);
    float bu2 = 3. * u * u * (1.-u);
    float bu3 = u * u * u;

    float bv0 = (1.-v) * (1.-v) * (1.-v);
    float bv1 = 3. * v * (1.-v) * (1.-v);
    float bv2 = 3. * v * v * (1.-v);
    float bv3 = v * v * v;

    // finally, we get to compute something:
    float height = 	  bu0 * ( bv0*Z00 + bv1*Z01 + bv2*Z02 + bv3*Z03 )
        + bu1 * ( bv0*Z10 + bv1*Z11 + bv2*Z12 + bv3*Z13 )
        + bu2 * ( bv0*Z20 + bv1*Z21 + bv2*Z22 + bv3*Z23 )
        + bu3 * ( bv0*Z30 + bv1*Z31 + bv2*Z32 + bv3*Z33 );

    return height;
}


int main( int argc, char *argv[ ] ) {
    //. . .
    float volume = 0;

    // the area of a single full-sized tile:
    float fullTileArea = (  ( (XMAX-XMIN)/(float)NUMS )  *  ( ( YMAX - YMIN )/(float)NUMS )  );

    // sum up the weighted heights into the variable "volume"
    // using an OpenMP for loop and an addition reduction:

    //	?????

    for( int i = 0; i < NUMS*NUMS; ++i )	{
        int iu = i % NUMS;
        int iv = i / NUMS;

        if((iv == 0 || iv == NUMS - 1) && (iu == 0 || iu == NUMS - 1)) {

            // if true we are dealing with a corner.
            volume += ((fullTileArea / 4) * Height(iu, iv) );
            //printf("%d\t%d\t%d\tC\n", i, iu, iv); //for debugging
        } else if ((iv == 0 || iv == NUMS - 1) || (iu == 0 || iu == NUMS - 1)) {

            // if true then we are dealing with the first or last row
            volume += ((fullTileArea / 2) * Height(iu, iv) );
            //printf("%d\t%d\t%d\tS\n", i, iu, iv); // for debugging
        } else {

            // we are dealing with a center piece 
            volume += (fullTileArea * Height(iu, iv) );
            //printf("%d\t%d\t%d\n", i, iu, iv); // for debugging
        }
    }

    //	. . .
    printf("%f\n", volume);
}
