#ifndef _BIT_MAP_H
#define _BIT_MAP_H

#define BITMAP_WORD 32   // 32 bits
#define BITMAP_SHIFT 5 ////�ƶ�5��λ,�������൱�ڳ���32,�����൱�ڳ���32ȡ��
#define BITMAP_MASK 0x1F //16�����µ�31
#define BITMAP_N 10000000

static int _bitmap[1 + BITMAP_N / BITMAP_WORD];
/* 
 * ��λ����������"|"������,i&MASK�൱��mod���� 
 * m mod n ���㣬��n = 2��X���ݵ�ʱ��,m mod n = m&(n-1)
 */
static inline void bitmap_set(int i) {
    _bitmap[i >> BITMAP_SHIFT] |= (1 << (i & BITMAP_MASK));
}  
/* ���λ��������&~������ */
static inline void bitmap_clear(int i) {
    _bitmap[i >> BITMAP_SHIFT] &= ~(1 << (i & BITMAP_MASK));
}

/* ����λ������&������ */  
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