#include "bs_memory.h"
#include "bs_datetime.h"

#if bsPlatform == bsPlatform_Win32
#include <pthread.h>
#else
#   if bsPlatform == bsPlatform_Apple
#include <sys/types.h>
#include <sys/sysctl.h>
#else
#include <string.h>
#include <sched.h>
#   endif
#endif

#define SSE_MMREG_SIZE 16
#define MMX_MMREG_SIZE 8

#define MMX1_MIN_LEN 0x800  /* 2K blocks */
#define MIN_LEN 0x40  /* 64-byte blocks */


#define cpuid(index, eax, ebx, ecx, edx)\
    __asm __volatile\
    ("mov %%ebx, %%esi\n\t"\
    "cpuid\n\t"\
    "xchg %%ebx, %%esi"\
    : "=a" (eax), "=S" (ebx),\
    "=c" (ecx), "=d" (edx)\
    : "0" (index));
/* memcpy */
/* for small memory blocks (<256 bytes) this version is faster */
#   define small_memcpy(to,from,n)\
{\
    register ulong dummy;\
    __asm__ __volatile__(\
    "rep; movsb"\
    :"=&D"(to), "=&S"(from), "=&c"(dummy)\
    :"0" (to), "1" (from),"2" (n)\
    : "memory");\
    }

/* linux kernel __memcpy (from: /include/asm/string.h) */
static __inline__ void * linux_kernel_memcpy_impl (void * to, const void * from, size_t n)
{
    int d0, d1, d2;

    if (n < 4){
        small_memcpy(to, from, n);
    }else
    {
        __asm__ __volatile__(
                    "rep ; movsl\n\t"
                    "testb $2,%b4\n\t"
                    "je 1f\n\t"
                    "movsw\n"
                    "1:\ttestb $1,%b4\n\t"
                    "je 2f\n\t"
                    "movsb\n"
                    "2:"
                    : "=&c" (d0), "=&D" (d1), "=&S" (d2)
                    :"0" (n/4), "q" (n),"1" ((intptr) to),"2" ((intptr) from)
                    : "memory");
    }

    return (to);
}

bs_memory::bs_memory(eMemcpyModes m):
    mode(m)
{
    if (m == Memcpy_Auto)
        mode = probe();
}
void *bs_memory::copy(void *dst, const void* src, int64 len)
{
    if (mode & CPU_SSE)
        return copy_sse(dst, src, len);
    else if (mode & CPU_MMX)
        return copy_mmx(dst, src, len);
    else if (mode & CPU_MMXEXT)
        return copy_mmx2(dst, src, len);
    return memcpy (dst, src, len);
}
void *bs_memory::move(void *dst, const void* src, int64 len)
{
    return move_c(dst, src, len);
}
void *bs_memory::set(void *dst, int val, int64 len)
{
    return set_c(dst, val, len);
}
void *bs_memory::move_c(void *dst, const void* src, int64 len)
{
    return memmove(dst, src, len);
}
void *bs_memory::set_c(void *dat, int val, int64 len)
{
    return memset(dat, val, len);
}

void * bs_memory::copy_sse(void *dst, const void *src, int64 len)
{
    void *retval;
    uint8 *ufrom=(uint8 *)src;
    uint8 *uto=(uint8 *)dst;
    retval = uto;

    /* PREFETCH has effect even for MOVSB instruction ;) */
    __asm__ __volatile__ (
                "   prefetchnta (%0)\n"
                "   prefetchnta 32(%0)\n"
                "   prefetchnta 64(%0)\n"
                "   prefetchnta 96(%0)\n"
                "   prefetchnta 128(%0)\n"
                "   prefetchnta 160(%0)\n"
                "   prefetchnta 192(%0)\n"
                "   prefetchnta 224(%0)\n"
                "   prefetchnta 256(%0)\n"
                "   prefetchnta 288(%0)\n"
                : : "r" (ufrom) );

    if(len >= MIN_LEN)
    {
        register ulong delta;
        /* Align destinition to MMREG_SIZE -boundary */
        delta = ((intptr_t)uto)&(SSE_MMREG_SIZE-1);
        if(delta)
        {
            delta = SSE_MMREG_SIZE-delta;
            len -= delta;
            small_memcpy(uto, ufrom, delta);
        }
        size_t i = len >> 6; /* len/64 */
        len &= 63;
        if(((intptr_t)ufrom) & 15)
            /* if SRC is misaligned */
            for(; i > 0; i--)
            {
                __asm__ __volatile__ (
                            "prefetchnta 320(%0)\n"
                            "prefetchnta 352(%0)\n"
                            "movups (%0), %%xmm0\n"
                            "movups 16(%0), %%xmm1\n"
                            "movups 32(%0), %%xmm2\n"
                            "movups 48(%0), %%xmm3\n"
                            "movntps %%xmm0, (%1)\n"
                            "movntps %%xmm1, 16(%1)\n"
                            "movntps %%xmm2, 32(%1)\n"
                            "movntps %%xmm3, 48(%1)\n"
                            :: "r" (ufrom), "r" (uto) : "memory");
                ufrom += 64;
                uto += 64;
            }
        else
            /*
           Only if SRC is aligned on 16-byte boundary.
           It allows to use movaps instead of movups, which required data
           to be aligned or a general-protection exception (#GP) is generated.
        */
            for(; i > 0; i--)
            {
                __asm__ __volatile__ (
                            "prefetchnta 320(%0)\n"
                            "prefetchnta 352(%0)\n"
                            "movaps (%0), %%xmm0\n"
                            "movaps 16(%0), %%xmm1\n"
                            "movaps 32(%0), %%xmm2\n"
                            "movaps 48(%0), %%xmm3\n"
                            "movntps %%xmm0, (%1)\n"
                            "movntps %%xmm1, 16(%1)\n"
                            "movntps %%xmm2, 32(%1)\n"
                            "movntps %%xmm3, 48(%1)\n"
                            :: "r" (ufrom), "r" (uto) : "memory");
                ufrom += 64;
                uto += 64;
            }
        /* since movntq is weakly-ordered, a "sfence"
       * is needed to become ordered again. */
        __asm__ __volatile__ ("sfence":::"memory");
        /* enables to use FPU */
        __asm__ __volatile__ ("emms":::"memory");
    }
    /*
     *	Now do the tail of the block
     */
    if(len)
        linux_kernel_memcpy_impl(uto, ufrom, len);
    return retval;
}
void * bs_memory::copy_mmx(void * dst, const void * src, int64 len)
{
    void *retval;
    uint8 *ufrom=(uint8 *)src;
    uint8 *uto=(uint8 *)dst;
    retval = uto;

    if (len >= MMX1_MIN_LEN)
    {
        register ulong delta;
        /* Align destinition to MMREG_SIZE -boundary */
        delta = ((intptr)uto)&(MMX_MMREG_SIZE-1);
        if (delta)
        {
            delta = MMX_MMREG_SIZE-delta;
            len -= delta;
            small_memcpy(uto, ufrom, delta);
        }
        size_t i = len >> 6; /* len/64 */
        len&=63;
        for (; i > 0; i--)
        {
            __asm__ __volatile__ (
                        "movq (%0), %%mm0\n"
                        "movq 8(%0), %%mm1\n"
                        "movq 16(%0), %%mm2\n"
                        "movq 24(%0), %%mm3\n"
                        "movq 32(%0), %%mm4\n"
                        "movq 40(%0), %%mm5\n"
                        "movq 48(%0), %%mm6\n"
                        "movq 56(%0), %%mm7\n"
                        "movq %%mm0, (%1)\n"
                        "movq %%mm1, 8(%1)\n"
                        "movq %%mm2, 16(%1)\n"
                        "movq %%mm3, 24(%1)\n"
                        "movq %%mm4, 32(%1)\n"
                        "movq %%mm5, 40(%1)\n"
                        "movq %%mm6, 48(%1)\n"
                        "movq %%mm7, 56(%1)\n"
                        :: "r" (ufrom), "r" (uto) : "memory");
            ufrom += 64;
            uto += 64;
        }
        __asm__ __volatile__ ("emms":::"memory");
    }
    /*
   *	Now do the tail of the block
   */
    if(len)
        linux_kernel_memcpy_impl(uto, ufrom, len);
    return retval;
}
void * bs_memory::copy_mmx2(void * dst, const void * src, int64 len)
{
    void *retval;
    uint8 *ufrom=(uint8 *)src;
    uint8 *uto=(uint8 *)dst;
    retval = uto;

    /* PREFETCH has effect even for MOVSB instruction ;) */
    __asm__ __volatile__ (
                "   prefetchnta (%0)\n"
                "   prefetchnta 32(%0)\n"
                "   prefetchnta 64(%0)\n"
                "   prefetchnta 96(%0)\n"
                "   prefetchnta 128(%0)\n"
                "   prefetchnta 160(%0)\n"
                "   prefetchnta 192(%0)\n"
                "   prefetchnta 224(%0)\n"
                "   prefetchnta 256(%0)\n"
                "   prefetchnta 288(%0)\n"
                : : "r" (ufrom) );

    if(len >= MIN_LEN)
    {
        register ulong delta;
        /* Align destinition to MMREG_SIZE -boundary */
        delta = ((intptr)uto)&(MMX_MMREG_SIZE-1);
        if(delta)
        {
            delta = MMX_MMREG_SIZE-delta;
            len -= delta;
            small_memcpy(uto, ufrom, delta);
        }
        size_t i = len >> 6; /* len/64 */
        len&=63;
        for(; i > 0; i--)
        {
            __asm__ __volatile__ (
                        "prefetchnta 320(%0)\n"
                        "prefetchnta 352(%0)\n"
                        "movq (%0), %%mm0\n"
                        "movq 8(%0), %%mm1\n"
                        "movq 16(%0), %%mm2\n"
                        "movq 24(%0), %%mm3\n"
                        "movq 32(%0), %%mm4\n"
                        "movq 40(%0), %%mm5\n"
                        "movq 48(%0), %%mm6\n"
                        "movq 56(%0), %%mm7\n"
                        "movntq %%mm0, (%1)\n"
                        "movntq %%mm1, 8(%1)\n"
                        "movntq %%mm2, 16(%1)\n"
                        "movntq %%mm3, 24(%1)\n"
                        "movntq %%mm4, 32(%1)\n"
                        "movntq %%mm5, 40(%1)\n"
                        "movntq %%mm6, 48(%1)\n"
                        "movntq %%mm7, 56(%1)\n"
                        :: "r" (ufrom), "r" (uto) : "memory");
            ufrom += 64;
            uto += 64;
        }
        /* since movntq is weakly-ordered, a "sfence"
     * is needed to become ordered again. */
        __asm__ __volatile__ ("sfence":::"memory");
        __asm__ __volatile__ ("emms":::"memory");
    }
    /*
   *	Now do the tail of the block
   */
    if(len)
        linux_kernel_memcpy_impl(uto, ufrom, len);
    return retval;
}

bool bs_memory::have_cpuid()const
{
#if bsArchitecture == bsArchitecture_64Bit
    return true;
#endif

    long a, c;
    __asm__ __volatile__ (
                "pushf\n\t"
                "pop %0\n\t"
                "mov %0, %1\n\t"

                "xor $0x200000, %0\n\t"
                "push %0\n\t"
                "popf\n\t"

                "pushf\n\t"
                "pop %0\n\t"
                : "=a" (a), "=c" (c)
                :
                : "cc"
                );

    if (a == c)
        return false;
    return true;
}

eMemcpyModes bs_memory::probe(int64 size, uint64 *time) const
{
    void *src,*dst;
    uint64_t start, stop, length;
    eMemcpyModes rets = Memcpy_Default;

    src = bsMalloc (size);
    dst = bsMalloc (size);

    // default
    start = bs_datetime::microsecond();
    memcpy (dst, src, size);
    stop = bs_datetime::microsecond();
    length = stop - start;

    // sse
    if (have_capable(CPU_SSE))
    {
        start = bs_datetime::microsecond();
        copy_sse (dst, src, size);
        stop = bs_datetime::microsecond();
        if ((stop - start) < length)
        {
            rets = Memcpy_SSE;
            length = stop - start;
        }
    }
    // mmx
    if (have_capable(CPU_MMX))
    {
        start = bs_datetime::microsecond();
        copy_mmx (dst, src, size);
        stop = bs_datetime::microsecond();
        if ((stop - start) < length)
        {
            rets = Memcpy_MMX;
            length = stop - start;
        }
    }
    // mmx2
    if (have_capable(CPU_MMXEXT))
    {
        start = bs_datetime::microsecond();
        copy_mmx2 (dst, src, size);
        stop = bs_datetime::microsecond();
        if ((stop - start) < length)
        {
            rets = Memcpy_MMX2;
            length = stop - start;
        }
    }
    bsFree(src);
    bsFree(dst);

    if (time)
        *time = length;
    return rets;
}
int bs_memory::capable()const
{
    if (have_cpuid())
    {
        int cpucap = 0;
        int eax, ebx, ecx, edx;
        int max_std_level, max_ext_level;
        cpuid(0, max_std_level, ebx, ecx, edx);

        if(max_std_level >= 1)
        {
            int std_caps = 0;
            cpuid(1, eax, ebx, ecx, std_caps);
            if (std_caps & (1<<23))
                cpucap |= CPU_MMX;
            if (std_caps & (1<<25))
                cpucap |= CPU_MMXEXT | CPU_SSE;
            if (std_caps & (1<<26))
                cpucap |= CPU_SSE2;
            if (ecx & 1)
                cpucap |= CPU_SSE3;
            if (ecx & 0x00000200 )
                cpucap |= CPU_SSSE3;
        }

        cpuid(0x80000000, max_ext_level, ebx, ecx, edx);

        if(max_ext_level >= 0x80000001)
        {
            int ext_caps = 0;
            cpuid(0x80000001, eax, ebx, ecx, ext_caps);
            if (ext_caps & (1<<31))
                cpucap |= CPU_3DNOW;
            if (ext_caps & (1<<30))
                cpucap |= CPU_3DNOWEXT;
            if (ext_caps & (1<<23))
                cpucap |= CPU_MMX;
            if (ext_caps & (1<<22))
                cpucap |= CPU_MMXEXT;
        }
        return cpucap;
    }
    return CPU_Unknown;
}
int bs_memory::cpu_count()const
{
#if bsPlatform == bsPlatform_Win32
    return pthread_num_processors_np();
#endif

#if bsPlatform == bsPlatform_Apple
    int np;
    size_t length = sizeof(np);
    if (sysctlbyname("hw.ncpu", &np, &length, NULL, 0))
        np = 1;

    return np;
#endif

#if bsPlatform == bsPlatform_Unix || bsPlatform == bsPlatform_Linux
    uint bit;
    int np;

    cpu_set_t p_aff;
    memset( &p_aff, 0, sizeof(p_aff) );
    sched_getaffinity( 0, sizeof(p_aff), &p_aff );

    for( np = 0, bit = 0; bit < sizeof(p_aff); bit++ )
        np += (((uint8 *)&p_aff)[bit / 8] >> (bit % 8)) & 1;

    return np;
#endif
}
bool bs_memory::have_capable(eCPUCapables cap)const
{
    return capable() & cap;
}
