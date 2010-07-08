/* ============================================================
Copyright (c) 2002 Advanced Micro Devices, Inc.

All rights reserved.

Redistribution and  use in source and binary  forms, with or
without  modification,  are   permitted  provided  that  the
following conditions are met:

+ Redistributions  of source  code  must  retain  the  above
  copyright  notice,   this  list  of   conditions  and  the
  following disclaimer.

+ Redistributions  in binary  form must reproduce  the above
  copyright  notice,   this  list  of   conditions  and  the
  following  disclaimer in  the  documentation and/or  other
  materials provided with the distribution.

+ Neither the  name of Advanced Micro Devices,  Inc. nor the
  names  of  its contributors  may  be  used  to endorse  or
  promote  products  derived   from  this  software  without
  specific prior written permission.

THIS  SOFTWARE  IS PROVIDED  BY  THE  COPYRIGHT HOLDERS  AND
CONTRIBUTORS "AS IS" AND  ANY EXPRESS OR IMPLIED WARRANTIES,
INCLUDING,  BUT NOT  LIMITED TO,  THE IMPLIED  WARRANTIES OF
MERCHANTABILITY  AND FITNESS  FOR A  PARTICULAR  PURPOSE ARE
DISCLAIMED.  IN  NO  EVENT  SHALL  ADVANCED  MICRO  DEVICES,
INC.  OR CONTRIBUTORS  BE LIABLE  FOR ANY  DIRECT, INDIRECT,
INCIDENTAL,  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES
(INCLUDING,  BUT NOT LIMITED  TO, PROCUREMENT  OF SUBSTITUTE
GOODS  OR  SERVICES;  LOSS  OF  USE, DATA,  OR  PROFITS;  OR
BUSINESS INTERRUPTION)  HOWEVER CAUSED AND ON  ANY THEORY OF
LIABILITY,  WHETHER IN CONTRACT,  STRICT LIABILITY,  OR TORT
(INCLUDING NEGLIGENCE  OR OTHERWISE) ARISING IN  ANY WAY OUT
OF  THE  USE  OF  THIS  SOFTWARE, EVEN  IF  ADVISED  OF  THE
POSSIBILITY OF SUCH DAMAGE.

It is  licensee's responsibility  to comply with  any export
regulations applicable in licensee's jurisdiction.
============================================================ */

#include "libm_amd.h"
#include "libm_util_amd.h"

#define USE_VAL_WITH_FLAGS
#define USE_NAN_WITH_FLAGS
#define USE_HANDLE_ERROR
#include "libm_inlines_amd.h"
#undef USE_NAN_WITH_FLAGS
#undef USE_VAL_WITH_FLAGS
#undef USE_HANDLE_ERROR

#include "libm_errno_amd.h"
#include "lib_version.h"

/* Deal with errno for out-of-range argument */
static inline double retval_errno_edom(double x)
{
  struct exception exc;
  exc.arg1 = x;
  exc.arg2 = x;
  exc.type = DOMAIN;
  exc.name = (char *)"asin";
  if (PATH64_LIB_VERSION_SVID)
    exc.retval = HUGE;
  else
    exc.retval = nan_with_flags(AMD_F_INVALID);
  if (PATH64_LIB_VERSION_POSIX)
    __set_errno(EDOM);
  else if (!matherr(&exc))
    {
      if(PATH64_LIB_VERSION_SVID)
        (void)fputs("asin: DOMAIN error\n", stderr);
    __set_errno(EDOM);
    }
  return exc.retval;
}


double FN_PROTOTYPE(asin)(double x)
{
  /* Computes arcsin(x).
     The argument is first reduced by noting that arcsin(x) 
     is invalid for abs(x) > 1 and arcsin(-x) = -arcsin(x).
     For denormal and small arguments arcsin(x) = x to machine
     accuracy. Remaining argument ranges are handled as follows. 
     For abs(x) <= 0.5 use 
     arcsin(x) = x + x^3*R(x^2)
     where R(x^2) is a rational minimax approximation to 
     (arcsin(x) - x)/x^3.
     For abs(x) > 0.5 exploit the identity:
      arcsin(x) = pi/2 - 2*arcsin(sqrt(1-x)/2)
     together with the above rational approximation, and 
     reconstruct the terms carefully.
    */

  /* Some constants and split constants. */

  static const double 
    piby2_tail  = 6.1232339957367660e-17, /* 0x3c91a62633145c07 */
    hpiby2_head = 7.8539816339744831e-01, /* 0x3fe921fb54442d18 */
    piby2       = 1.5707963267948965e+00; /* 0x3ff921fb54442d18 */
  double u, v, y, s=0.0, r;
  int xexp, xnan, transform=0;

  unsigned long ux, aux, xneg;
  GET_BITS_DP64(x, ux);
  aux = ux & ~SIGNBIT_DP64;
  xneg = (ux & SIGNBIT_DP64);
  xnan = (aux > PINFBITPATT_DP64);
  xexp = (int)((ux & EXPBITS_DP64) >> EXPSHIFTBITS_DP64) - EXPBIAS_DP64;

  /* Special cases */

  if (xnan)
    {
      return x + x; /* With invalid if it's a signalling NaN */
    }
  else if (xexp < -28)
    { /* y small enough that arcsin(x) = x */
      return val_with_flags(x, AMD_F_INEXACT);
    }
  else if (xexp >= 0) 
    { /* abs(x) >= 1.0 */
      if (x == 1.0)
        return val_with_flags(piby2, AMD_F_INEXACT);
      else if (x == -1.0)
        return val_with_flags(-piby2, AMD_F_INEXACT);
      else
        return retval_errno_edom(x);
    }

  if (xneg) y = -x;
  else y = x;

  transform = (xexp >= -1); /* abs(x) >= 0.5 */

  if (transform)
    { /* Transform y into the range [0,0.5) */
      r = 0.5*(1.0 - y);
#ifdef WINDOWS
      /* VC++ intrinsic call */
      _mm_store_sd(&s, _mm_sqrt_sd(_mm_setzero_pd(), _mm_load_sd(&r)));
#else
      /* Hammer sqrt instruction */
      asm volatile ("sqrtsd %1, %0" : "=x" (s) : "x" (r));
#endif
      y = s;
    }
  else
      r = y*y;

  /* Use a rational approximation for [0.0, 0.5] */

  u = r*(0.227485835556935010735943483075 + 
         (-0.445017216867635649900123110649 +
          (0.275558175256937652532686256258 + 
           (-0.0549989809235685841612020091328 +
            (0.00109242697235074662306043804220 + 
             0.0000482901920344786991880522822991*r)*r)*r)*r)*r)/
    (1.36491501334161032038194214209 +
     (-3.28431505720958658909889444194 + 
      (2.76568859157270989520376345954 + 
       (-0.943639137032492685763471240072 +
	0.105869422087204370341222318533*r)*r)*r)*r);

  if (transform) 
    { /* Reconstruct asin carefully in transformed region */
      	{
	  double c, s1, p, q;
	  unsigned long us;
	  GET_BITS_DP64(s, us);
	  PUT_BITS_DP64(0xffffffff00000000 & us, s1);
	  c = (r-s1*s1)/(s+s1);
          p = 2.0*s*u - (piby2_tail-2.0*c);
          q = hpiby2_head - 2.0*s1;
          v = hpiby2_head - (p-q);
	}
    }
  else
    {
      v = y + y*u;
    }

  if (xneg) return -v;
  else return v;
}

weak_alias (__asin, asin)
