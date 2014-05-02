#ifndef COMPAT_H_
#define COMPAT_H_

#ifdef _WINDOWS

#undef strdup
#define strdup _strdup

#endif // _WINDOWS

#endif // COMPAT_H_
