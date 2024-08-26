/*
    Copyright 2011, Spyros Blanas.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef LOCK_H
#define LOCK_H

typedef volatile char Lock_t;

inline void unlock(Lock_t * _l) __attribute__((always_inline));
inline void lock(Lock_t * _l) __attribute__((always_inline));
inline int  tas(volatile char * lock) __attribute__((always_inline));

/*
 * Non-recursive spinlock. Using `xchg` and `ldstub` as in PostgresSQL.
 */
/* Call blocks and retunrs only when it has the lock. */
inline void lock(Lock_t * _l) {
    while(tas(_l)) {
#if defined(__i386__) || defined(__x86_64__)
        __asm__ __volatile__ ("pause\n");
#endif
    }
}

/** Unlocks the lock object. */
inline void unlock(Lock_t * _l) { 
    *_l = 0;
}

inline int tas(volatile char *lock)
{
    // For C++17 and later, `register` is deprecated. Use a regular variable.
#if __cplusplus >= 201703L
    char res = 1;
#else
    register char res = 1;
#endif // __cplusplus >= 201703L

#if defined(__aarch64__)
    unsigned int tmp;
    __asm__ __volatile__ (
                          "ldaxrb %w0, [%2]\n"    // Load the value from memory into tmp (byte)
                          "eor %w0, %w0, #1\n"    // Compute the new value (exclusive OR with 1)
                          "stlxrb %w1, %w0, [%2]\n" // Store the new value into memory with exclusive store
                          : "=&r"(res), "=&r"(tmp)
                          : "r"(lock)
                          : "memory", "cc");

#elif defined(__i386__) || defined(__x86_64__)
    __asm__ __volatile__ (
                          "lock xchgb %0, %1\n"
                          : "+q"(res), "+m"(*lock)
                          :
                          : "memory", "cc");

#else
#error TAS not defined for this architecture.
#endif

    return res;
}





#endif /* LOCK_H */
