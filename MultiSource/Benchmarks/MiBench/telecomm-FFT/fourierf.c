/*============================================================================

    fourierf.c  -  Don Cross <dcross@intersrv.com>

    http://www.intersrv.com/~dcross/fft.html

    Contains definitions for doing Fourier transforms
    and inverse Fourier transforms.

    This module performs operations on arrays of 'float'.

    Revision history:

1998 September 19 [Don Cross]
    Updated coding standards.
    Improved efficiency of trig calculations.

============================================================================*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "fourier.h"
#include "ddcmath.h"

#define CHECKPOINTER(p)  CheckPointer(p,#p)

const double SinPi4Result = 0x1.6a09e667f3bccp-1;
const double SinPi16Result = 0x1.8f8b83c69a60ap-3;
const double CosPi4Result = 0x1.6a09e667f3bcdp-1;

static void CheckPointer ( void *p, char *name )
{
    if ( p == NULL )
    {
        fprintf ( stderr, "Error in fft_float():  %s == NULL\n", name );
        exit(1);
    }
}


void fft_float (
    unsigned  NumSamples,
    int       InverseTransform,
    float    *RealIn,
    float    *ImagIn,
    float    *RealOut,
    float    *ImagOut )
{
    unsigned NumBits;    /* Number of bits needed to store indices */
    unsigned i, j, k, n;
    unsigned BlockSize, BlockEnd;

    double angle_numerator = 2.0 * DDC_PI;
    double tr, ti;     /* temp real, temp imaginary */

    if ( !IsPowerOfTwo(NumSamples) )
    {
        fprintf (
            stderr,
            "Error in fft():  NumSamples=%u is not power of two\n",
            NumSamples );

        exit(1);
    }

    if ( InverseTransform )
        angle_numerator = -angle_numerator;

    CHECKPOINTER ( RealIn );
    CHECKPOINTER ( RealOut );
    CHECKPOINTER ( ImagOut );

    NumBits = NumberOfBitsNeeded ( NumSamples );

    /*
    **   Do simultaneous data copy and bit-reversal ordering into outputs...
    */

    for ( i=0; i < NumSamples; i++ )
    {
        j = ReverseBits ( i, NumBits );
        RealOut[j] = RealIn[i];
        ImagOut[j] = (ImagIn == NULL) ? 0.0 : ImagIn[i];
    }

    /*
    **   Do the FFT itself...
    */

    BlockEnd = 1;
    for ( BlockSize = 2; BlockSize <= NumSamples; BlockSize <<= 1 )
    {
        double delta_angle = angle_numerator / (double)BlockSize;
        double sm2 = sin ( -2 * delta_angle );
        double sm1 = sin ( -delta_angle );
        double cm2 = cos ( -2 * delta_angle );
        double cm1 = cos ( -delta_angle );
        // The sin and cos functions implemented in the AIX libm math
        // library produce slightly less accurate results compared to sin
        // and cos in Linux libm for some inputs; a difference of 1ulp.
        // For sin, the results differ between AIX and Linux libm for inputs
        // (pi/4) and (pi/16).
        // For cos, the results differ between AIX and Linux libm for (pi/4).
        // For these inputs specifically, the sin and cos results are set
        // to the specific results retrieved from Linux libm.
        if ((-2 * delta_angle) == (DDC_PI / 4)) {
          sm2 = SinPi4Result;
          cm2 = CosPi4Result;
        } else if ((-2 * delta_angle) == (DDC_PI / 16))
          sm2 = SinPi16Result;
        if ((-delta_angle) == (DDC_PI / 4)) {
          sm1 = SinPi4Result;
          cm1 = CosPi4Result;
        } else if ((-delta_angle) == (DDC_PI / 16))
          sm1 = SinPi16Result;
        double w = 2 * cm1;
        double ar[3], ai[3];
        double temp;

        for ( i=0; i < NumSamples; i += BlockSize )
        {
            ar[2] = cm2;
            ar[1] = cm1;

            ai[2] = sm2;
            ai[1] = sm1;

            for ( j=i, n=0; n < BlockEnd; j++, n++ )
            {
                ar[0] = w*ar[1] - ar[2];
                ar[2] = ar[1];
                ar[1] = ar[0];

                ai[0] = w*ai[1] - ai[2];
                ai[2] = ai[1];
                ai[1] = ai[0];

                k = j + BlockEnd;
                tr = ar[0]*RealOut[k] - ai[0]*ImagOut[k];
                ti = ar[0]*ImagOut[k] + ai[0]*RealOut[k];

                RealOut[k] = RealOut[j] - tr;
                ImagOut[k] = ImagOut[j] - ti;

                RealOut[j] += tr;
                ImagOut[j] += ti;
            }
        }

        BlockEnd = BlockSize;
    }

    /*
    **   Need to normalize if inverse transform...
    */

    if ( InverseTransform )
    {
        double denom = (double)NumSamples;

        for ( i=0; i < NumSamples; i++ )
        {
            RealOut[i] /= denom;
            ImagOut[i] /= denom;
        }
    }
}

void fft_float_StrictFP (
    unsigned  NumSamples,
    int       InverseTransform,
    float    *RealIn,
    float    *ImagIn,
    float    *RealOut,
    float    *ImagOut )
{
#pragma STDC FP_CONTRACT OFF
    unsigned NumBits;    /* Number of bits needed to store indices */
    unsigned i, j, k, n;
    unsigned BlockSize, BlockEnd;

    double angle_numerator = 2.0 * DDC_PI;
    double tr, ti;     /* temp real, temp imaginary */

    if ( !IsPowerOfTwo(NumSamples) )
    {
        fprintf (
            stderr,
            "Error in fft():  NumSamples=%u is not power of two\n",
            NumSamples );

        exit(1);
    }

    if ( InverseTransform )
        angle_numerator = -angle_numerator;

    CHECKPOINTER ( RealIn );
    CHECKPOINTER ( RealOut );
    CHECKPOINTER ( ImagOut );

    NumBits = NumberOfBitsNeeded ( NumSamples );

    /*
    **   Do simultaneous data copy and bit-reversal ordering into outputs...
    */

    for ( i=0; i < NumSamples; i++ )
    {
        j = ReverseBits ( i, NumBits );
        RealOut[j] = RealIn[i];
        ImagOut[j] = (ImagIn == NULL) ? 0.0 : ImagIn[i];
    }

    /*
    **   Do the FFT itself...
    */

    BlockEnd = 1;
    for ( BlockSize = 2; BlockSize <= NumSamples; BlockSize <<= 1 )
    {
        double delta_angle = angle_numerator / (double)BlockSize;
        double sm2 = sin ( -2 * delta_angle );
        double sm1 = sin ( -delta_angle );
        double cm2 = cos ( -2 * delta_angle );
        double cm1 = cos ( -delta_angle );
        // The sin and cos functions implemented in the AIX libm math
        // library produce slightly less accurate results compared to sin
        // and cos in Linux libm for some inputs; a difference of 1ulp.
        // For sin, the results differ between AIX and Linux libm for inputs
        // (pi/4) and (pi/16).
        // For cos, the results differ between AIX and Linux libm for (pi/4).
        // For these inputs specifically, the sin and cos results are set
        // to the specific results retrieved from Linux libm.
        if ((-2 * delta_angle) == (DDC_PI / 4)) {
          sm2 = SinPi4Result;
          cm2 = CosPi4Result;
        } else if ((-2 * delta_angle) == (DDC_PI / 16))
          sm2 = SinPi16Result;
        if ((-delta_angle) == (DDC_PI / 4)) {
          sm1 = SinPi4Result;
          cm1 = CosPi4Result;
        } else if ((-delta_angle) == (DDC_PI / 16))
          sm1 = SinPi16Result;
        double w = 2 * cm1;
        double ar[3], ai[3];
        double temp;

        for ( i=0; i < NumSamples; i += BlockSize )
        {
            ar[2] = cm2;
            ar[1] = cm1;

            ai[2] = sm2;
            ai[1] = sm1;

            for ( j=i, n=0; n < BlockEnd; j++, n++ )
            {
                ar[0] = w*ar[1] - ar[2];
                ar[2] = ar[1];
                ar[1] = ar[0];

                ai[0] = w*ai[1] - ai[2];
                ai[2] = ai[1];
                ai[1] = ai[0];

                k = j + BlockEnd;
                tr = ar[0]*RealOut[k] - ai[0]*ImagOut[k];
                ti = ar[0]*ImagOut[k] + ai[0]*RealOut[k];

                RealOut[k] = RealOut[j] - tr;
                ImagOut[k] = ImagOut[j] - ti;

                RealOut[j] += tr;
                ImagOut[j] += ti;
            }
        }

        BlockEnd = BlockSize;
    }

    /*
    **   Need to normalize if inverse transform...
    */

    if ( InverseTransform )
    {
        double denom = (double)NumSamples;

        for ( i=0; i < NumSamples; i++ )
        {
            RealOut[i] /= denom;
            ImagOut[i] /= denom;
        }
    }
}


/*--- end of file fourierf.c ---*/
