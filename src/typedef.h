#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned long uint32_t;
typedef signed long int32_t;
typedef float float32_t;
typedef bit bool_t;
typedef enum
{
    FALSE,
    TRUE = !FALSE
}fastBool_t;

typedef void (*task_t)(void);

#endif



