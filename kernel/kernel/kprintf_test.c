#include <kernel/kstdio.h>
#include <stddef.h>
#include <stdint.h>

#define INT_MAX 0x7fffffff
#define INT_MIN (-INT_MAX - 1)

void kprintf_test_all(void) {
    kprintf("\n==============================\n");
    kprintf(" kprintf functional test suite\n");
    kprintf("==============================\n\n");

    /* 1. Basic literals & percent */
    // kprintf("[1] Basic literals & %%\n");
    // kprintf("Hello, kernel world!\n");
    // kprintf("100%% sure this works\n\n");

    /* 2. Character output & control chars */
    // kprintf("[2] Character output\n");
    // kprintf("Chars: %c %c %c\n", 'A', 'b', '0');
    // kprintf("Newline test%cLine2\n", '\n');
    // kprintf("Carriage return test\rXXX\n\n");

    /* 3. Signed decimal integers */
    // kprintf("[3] Signed integers\n");
    // kprintf("Signed: %d %d %d\n", 0, 42, -42);
    // kprintf("INT limits: %d %d\n\n", INT_MAX, INT_MIN);

    /* 4. Unsigned integers */
    // kprintf("[4] Unsigned integers\n");
    // kprintf("Unsigned: %u %u\n\n", 0u, 4294967295u);

    /* 5. Hexadecimal output */
    // kprintf("[5] Hexadecimal\n");
    // kprintf("Hex lowercase: %x\n", 0xdeadbeef);
    // kprintf("Hex uppercase: %X\n", 0xdeadbeef);
    // kprintf("Hex zero: %x\n\n", 0);

    /* 6. Length modifiers */
    // kprintf("[6] Length modifiers\n");
    // kprintf("hh: %hhd %hhu\n", (char)-5, (unsigned char)250);
    // kprintf("h:  %hd %hu\n", (short)-1234, (unsigned short)65535);
    // kprintf("l:  %ld %lu\n", -123456789L, 123456789UL);
    // kprintf("ll: %lld %llu\n", -1234567890123LL, 1234567890123ULL);
    // kprintf("j:  %jd %ju\n", (intmax_t)-1, (uintmax_t)-1);
    // kprintf("z:  %zd %zu\n", (ptrdiff_t)-1, (size_t)1234);
    // kprintf("t:  %td\n\n", (ptrdiff_t)-42);

    /* 7. Field width & justification */
    // kprintf("[7] Field width & justification\n");
    // kprintf("Right: '%5d'\n", 42);
    // kprintf("Left : '%-5d'\n", 42);
    // kprintf("Str right: '%10s'\n", "hi");
    // kprintf("Str left : '%-10s'\n\n", "hi");

    /* 8. Zero padding */
    // kprintf("[8] Zero padding\n");
    // kprintf("Zero pad: '%05d'\n", 42);
    // kprintf("Zero pad neg: '%05d'\n", -42);
    // kprintf("Left beats zero: '%-05d'\n\n", 42);

    /* 9. Integer precision */
    // kprintf("[9] Integer precision\n");
    // kprintf("Prec: '%.5d'\n", 42);
    // kprintf("Width+prec: '%8.5d'\n", 42);
    // kprintf("Zero precision zero: '%.0d'\n\n", 0);

    /* 10. String precision */
    kprintf("[10] String precision\n");
    kprintf("Str prec: '%.3s'\n", "abcdef");
    kprintf("Str width+prec: '%8.3s'\n\n", "abcdef");

    /* 11. NULL string handling */
    kprintf("[11] NULL string handling\n");
    kprintf("Null string: '%s'\n", (char *)0);
    kprintf("Null prec: '%.3s'\n\n", (char *)0);

    /* 12. Mixed-format stress test */
    kprintf("[12] Mixed-format stress test\n");
    kprintf(
        "Mix: c=%c d=%-6d u=%05u x=%08X s='%.4s'\n\n",
        'Z', -12, 34u, 0xBEEF, "kernel"
    );

    /* 13. Return value check */
    kprintf("[13] Return value check\n");
    int n = kprintf("Count me: %d %s\n", 123, "abc");
    kprintf("Written = %d\n\n", n);

    kprintf("========== END OF TEST ==========\n\n");
}

