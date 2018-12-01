/* bit.c
 * Author: Róbert Kolcún, FIT 
 * <xkolcu00@stud.fit.vutbr.cz>
 * Priprava na skusku z IJC
*/

//Pridanie N bitov na Y pozicii do premennej X
#define SETBITS(x, y, n) x = (x | (( 1 << n ) - 1) << y)

//Nulovanie N bitov na Y pozicii v premennej X
#define SETBITS(x, y, n) x = (x & (~(( 1 << n ) - 1) << y))

//Je to cislo
#define isNum(a) ((a >= ‘0’ && a <= ‘9’) ? (a - ‘0’) : -1)

//Je to pismeno
#define isChar(a) (a >= ‘a’ && a <= ‘z’ || a >= ‘A’ && a <= ‘Z’)

//Rotate Right
#define ROTATE_RIGHT(x,n) x = x >> (n % (sizeof(x)*8)) | x << (sizeof(x)*8 - (n % (sizeof(x)*8)))

//Change endian
#define CHANGE_ENDIAN(x) x = ((((x) & 0xFF) << 24) | (((x) & 0xFF00) << 8) | (((x) & 0xFF0000) >> 8) | ((x) >> 24))
//alebo
#define CHANGE_ENDIAN(x) x = ( (unsigned) ((((x) & 0xff000000) >> 24) | (((x) & 0x00ff0000) >> 8) | (((x) & 0x0000ff00) << 8)  | (((x) & 0x000000ff) << 24)))

//Swap bits
#define SWAP_BITS(x, a, b) \
  (x) = ((x) & (~(1 << (a))) & (~(1 << (b)))) | (((x) >> (a) & 1) << (b)) | (((x) >> (b) & 1) << (a));
