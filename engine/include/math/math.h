/*

  This file is a part of the Not Serious Engine
  project and distributed under the GNU GPL v3 license.

  Copyright © Kadir Aksoy
  https://github.com/kadir014/not-serious-engine

*/

#ifndef _NS_MATH_H
#define _NS_MATH_H

#include <math.h>


/**
 * @file math/math.h
 * 
 * @brief Generic mathematical constants and definitions.
 */


#define NS_PI 3.141592653589793238462643383279502884f
#define NS_DEG_TO_RAD 0.017453292519943295769236907684886127f

#define NS_RADIANS(a) ((a) * NS_DEG_TO_RAD)


/* For possible future reimplementations. */

#define ns_sqrt sqrtf
#define ns_sin sinf
#define ns_cos cosf
#define ns_tan tanf


#endif