#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

uint8_t get_bit(uint8_t b, uint8_t i){
    return (b & 0x01 << i) >> i;
}

int main(int argc, char ** argv){
    uint8_t tap1 = atoi(argv[1])-1;
    uint8_t tap2 = atoi(argv[2])-1;
    //printf("%d %d\n", tap1, tap2);

    if(tap2 < tap1){
        uint8_t tmp = tap1;
        tap1 = tap2;
        tap2 = tmp;
    }

    uint64_t MAX_LEN = pow(2, tap2+1) + 2 * tap2 + 1;
    printf("%ld\n", MAX_LEN);

    uint8_t nbyte = tap2 / 8 + 1;
    uint8_t *sr = malloc(sizeof(uint8_t) * nbyte);
    uint8_t *buf = malloc(sizeof(uint8_t) * MAX_LEN);

    for(uint8_t i=0; i < nbyte; i++) sr[i] = 0xff;

    for(uint64_t i=0; i < MAX_LEN; i++){
        uint8_t b1 = get_bit(sr[tap1/8], tap1%8);
        uint8_t b2 = get_bit(sr[tap2/8], tap2%8);
        uint8_t nb = b1^b2;
        
        for(uint8_t j=nbyte-1; j > 0; j--)
            sr[j] = sr[j] << 1 | get_bit(sr[j-1], 7);
        sr[0] = sr[0] << 1 | nb;
#ifdef DEBUG
        for(uint8_t j=0; j < nbyte; j++) printf("%08b", sr[nbyte-1-j]);
        printf("  %d\n", buf[i]);
#endif
        buf[i] = sr[nbyte-1] >> 7;
    }
    printf("Production done\n");
#ifdef DEBUG
    for(uint8_t j=0; j < MAX_LEN; j++) printf("%01b", buf[j]);
    printf("\n");
#endif

    for(uint64_t l = 1; l < MAX_LEN - tap2; l++){
        uint64_t sum = 0;
#ifdef DEBUG
        printf("=== %d ===\n", l);
#endif
        for(uint8_t j=0; j <= tap2; j++){
#ifdef DEBUG
            printf("%0b ^ %0b = %0b\n", buf[MAX_LEN - j - 1], buf[MAX_LEN - l - j - 1], buf[MAX_LEN - j - 1] ^ buf[MAX_LEN - l - j - 1]);
#endif
            sum += buf[MAX_LEN - j - 1] ^ buf[MAX_LEN - l - j - 1];
        }
        if(sum == 0){
            printf("Zero found for %d\n", l);
#ifdef DEBUG
            for(uint8_t j=0; j <= tap2; j++) printf("%01b", buf[MAX_LEN - j - 1]);
            printf("\n");
            for(uint8_t j=0; j <= tap2; j++) printf("%01b", buf[MAX_LEN - j - l - 1]);
            printf("\n");
#endif
            break;
        }
    }
    return 0;
}


