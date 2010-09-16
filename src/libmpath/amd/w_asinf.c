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

#define USE_VALF_WITH_FLAGS
#define USE_NANF_WITH_FLAGS
#define USE_HANDLE_ERRORF
#include "libm_inlines_amd.h"
#undef USE_NANF_WITH_FLAGS
#undef USE_VALF_WITH_FLAGS
#undef USE_HANDLE_ERRORF

#include "libm_errno_amd.h"
#include "lib_version.h"

/* Deal with errno for out-of-range argument */
static inline float retval_errno_edom(float x)
{
  struct exception exc;
  exc.arg1 = (double)x;
  exc.arg2 = (double)x;
  exc.type = DOMAIN;
  exc.name = (char *)"asinf";
  if (PATH64_LIB_VERSION_SVID)
    exc.retval = HUGE;
  else
    exc.retval = nanf_with_flags(AMD_F_INVALID);
  if (PATH64_LIB_VERSION_POSIX)
    __set_errno(EDOM);
  else if (!matherr(&exc))
    {
      if(PATH64_LIB_VERSION_SVID)
        (void)fputs("asinf: DOMAIN error\n", stderr);
    __set_errno(EDOM);
    }
  return exc.retval;
}

float FN_PROTOTYPE(asinf)(float x)
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

  static const float
    piby2_tail  = 7.5497894159e-08F, /* 0x33a22168 */
    hpiby2_head = 7.8539812565e-01F, /* 0x3f490fda */
    piby2       = 1.5707963705e+00F; /* 0x3fc90fdb */
  float u, v, y, s = 0.0F, r;
  int xexp, xnan, transform = 0;

  unsigned int ux, aux, xneg;
  GET_BITS_SP32(x, ux);
  aux = ux & ~SIGNBIT_SP32;
  xneg = (ux & SIGNBIT_SP32);
  xnan = (aux > PINFBITPATT_SP32);
  xexp = (int)((ux & EXPBITS_SP32) >> EXPSHIFTBITS_SP32) - EXPBIAS_SP32;

  /* Special cases */

  if (xnan)
    {
      return x + x; /* With invalid if it's a signalling NaN */
    }
  else if (xexp < -14)
    /* y small enough that arcsin(x) = x */
    return valf_with_flags(x, AMD_F_INEXACT);
  else if (xexp >= 0) 
    {
      /* abs(x) >= 1.0 */
      if (x == 1.0F)
        return valf_with_flags(piby2, AMD_F_INEXACT);
      else if (x == -1.0F)
        return valf_with_flags(-piby2, AMD_F_INEXACT);
      else
        return retval_errno_edom(x);
    }

  if (xneg) y = -x;
  else y = x;

  transform = (xexp >= -1); /* abs(x) >= 0.5 */

  if (transform)
    { /* Transform y into the range [0,0.5) */
      r = 0.5F*(1.0F - y);
#ifdef WINDOWS
      /* VC++ intrinsic call */
      _mm_store_ss(&s, _mm_sqrt_ss(_mm_load_ss(&r)));
#else
      /* Hammer sqrt instruction */
      asm volatile ("sqrtss %1, %0" : "=x" (s) : "x" (r));
#endif
      y = s;
    }
  else
    r = y*y;

  /* Use a rational approximation for [0.0, 0.5] */

  u=r*(0.184161606965100694821398249421F + 
       (-0.0565298683201845211985026327361F +
	(-0.0133819288943925804214011424456F - 
	 0.00396137437848476485201154797087F*r)*r)*r)/
    (1.10496961524520294485512696706F - 
     0.836411276854206731913362287293F*r);

  if (transform) 
    {
      /* Reconstruct asin carefully in transformed region */
      float c, s1, p, q;
      unsigned int us;
      GET_BITS_SP32(s, us);
      PUT_BITS_SP32(0xffff0000 & us, s1);
      c = (r-s1*s1)/(s+s1);
      p = 2.0F*s*u - (piby2_tail-2.0F*c);
      q = hpiby2_head - 2.0F*s1;
      v = hpiby2_head - (p-q);
    }
  else
    {
    v = y + y*u;
    }

  if (xneg) return -v;
  else return v;
}

weak_alias (__asinf, asinf)
