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

#define member(type, member) ((type *)0)->member
#define member_size(type, member) sizeof(((type *)0)->member)

#endif /* DATA_TYPES_H */