#ifndef DATA_TYPES_H
#define DATA_TYPES_H

typedef char i8;
typedef short int i16;
typedef int i32;
typedef long i64;

typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long u64;

typedef float f32;
typedef double f64;

// Some missing types from cglm
typedef unsigned int uvec2 [2];
typedef u8 u8vec2 [2];

typedef u8 u8vec3 [3];
typedef union {
	u8vec3 raw;
	struct {
		u8 x, y, z;
	};
} u8vec3s;

typedef u8 u8vec4 [4];
typedef union {
	u8vec4 raw;
	struct {
		u8 x, y, z, w;
	};
} u8vec4s;

#define member(type, member) ((type *)0)->member
#define member_size(type, member) sizeof(((type *)0)->member)

#endif /* DATA_TYPES_H */
