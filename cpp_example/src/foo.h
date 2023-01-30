#ifndef FOO_H
#define FOO_H

// C++ function prototypes go here
#ifdef __cplusplus

int cppFoo(void);

#endif

// C function prototypes go here
#ifdef __cplusplus
extern "C" {
#endif

int cFoo(void);

#ifdef __cplusplus
}
#endif

#endif /* FOO_H */
