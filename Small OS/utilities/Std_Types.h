/*
 * Std_Types.h
 *
 *  Created on: Apr 4, 2023
 *      Author: HAZEM-PC
 */

#ifndef UTILITIES_STD_TYPES_H_
#define UTILITIES_STD_TYPES_H_

/* Boolean Data Type */
typedef unsigned char boolean;

/* Boolean Values */
#ifndef FALSE
#define FALSE			(0u)
#endif
#ifndef TRUE
#define TRUE			(1u)
#endif

#define LOGIC_HIGH      (1u)
#define LOGIC_LOW       (0u)

#define NULL_PTR		((void*)0)


typedef unsigned char			u8;					/* 1 byte , 0 -> 255 */
typedef unsigned short int		u16;				/* 2 bytes, 0 -> 65,535 */
typedef unsigned long  int		u32;				/* 4 bytes, 0 -> 4,294,967,295 */
typedef unsigned long long int	u64;				/* 8 bytes, 0 -> 18,446,744,073,709,551,615 */

typedef signed char				s8;					/* 1 byte , -128 -> 127 */
typedef signed short int		s16;				/* 2 bytes, -32,768 -> 32,767 */
typedef signed long  int		s32;				/* 4 bytes, -2,147,483,648 -> 2,147,483,647 */
typedef signed long long int	s64;				/* 8 bytes, -9,223,372,036,854,775,807 -> 9,223,372,036,854,775,807 */

typedef float					f32;				/* 4 bytes, 3.4e-38 -> 3.4e+38 */
typedef double					f64;				/* 8 bytes, 1.7e-308 -> 1.7e+308 */

typedef void vd;

#define STD_TYPES_OK	1
#define STD_TYPES_NOK	0


#endif /* UTILITIES_STD_TYPES_H_ */


/*
typedef unsigned char         uint8;          //           0 .. 255
typedef signed char           sint8;          //        -128 .. +127
typedef unsigned short        uint16;         //           0 .. 65535
typedef signed short          sint16;         //      -32768 .. +32767
typedef unsigned long         uint32;         //           0 .. 4294967295
typedef signed long           sint32;         // -2147483648 .. +2147483647
typedef unsigned long long    uint64;         //       0 .. 18446744073709551615
typedef signed long long      sint64;         // -9223372036854775808 .. 9223372036854775807
typedef float                 float32;
typedef double                float64;
*/