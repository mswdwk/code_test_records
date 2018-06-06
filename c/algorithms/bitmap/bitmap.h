#ifndef _BIT_MAP_H
#define _BIT_MAP_H

#define BITMAP_WORD 32   // 32 bits
#define BITMAP_SHIFT 5 ////移动5个位,左移则相当于乘以32,右移相当于除以32取整
#define BITMAP_MASK 0x1F //16进制下的31
#define BITMAP_N 10000000

static int _bitmap[1 + BITMAP_N / BITMAP_WORD];
/* 
 * 置位函数——用"|"操作符,i&MASK相当于mod操作 
 * m mod n 运算，当n = 2的X次幂的时候,m mod n = m&(n-1)
 */
static inline void bitmap_set(int i) {
    _bitmap[i >> BITMAP_SHIFT] |= (1 << (i & BITMAP_MASK));
}  
/* 清除位操作，用&~操作符 */
static inline void bitmap_clear(int i) {
    _bitmap[i >> BITMAP_SHIFT] &= ~(1 << (i & BITMAP_MASK));
}

/* 测试位操作用&操作符 */  
static inline int bitmap_test(int i) {
    return _bitmap[i >> BITMAP_SHIFT] & (1 << (i & BITMAP_MASK));
}

static inline void bitmap_init(){
    #if 0
    int i;
    for (i = 0; i < BITMAP_N; i++) {
        clear(i);
    }
    #endif
    memset(_bitmap,0,sizeof(_bitmap));
}
#endif