#include <stdio.h>
#include <conio.h>
#include <Windows.h>

int main()
{
    // 27 bit is limit for 4 bytes unsigned int system
    // from (n * (1 << n)) < UINT_MAX, maximum n is 27
    // calculation need nearly 4 times for each bit increase
    int start_bit;
    char dummy;

    printf("Start bits: ");
    scanf_s("%d", &start_bit);
    while (_kbhit()) dummy = _getch();
    
    _putch('\n');

    if (start_bit < 2) start_bit = 2;
    else if (start_bit > 27) start_bit = 27;

    printf("Avg clock Table\nbits,C0 = 0,C0 = 1,All case,Add 1,Sub 1\n");

    for (int i = start_bit; i <= 27; i++) {
        unsigned int limit = 1 << i;
        double avgClk0 = 0.0;
        double avgClk1 = 0.0;

        // average clock for all case
        for (unsigned int x = 0; x < limit; x++) {
            unsigned int maxStack0 = 0;
            unsigned int maxStack1 = 0;

            for (unsigned int y = 0; y < limit; y++) {
                // C0 is 0 case
                int _xor0 = x ^ y;
                int _and0 = x & y;
                int max0 = 0;
                int count0 = 0;
                int bCounting0 = 0;

                // C0 is 1 case
                int _xor1 = ((x ^ y) << 1);
                int _and1 = ((x & y) << 1) + 0x1;
                int max1 = 0;
                int count1 = 0;
                int bCounting1 = 0;

                // C0 is 0
                // z = 1: last bit check is ignored
                for (int z = 1; z < i; z++) {
                    if (bCounting0) {
                        if(_xor0 & 0x1) {
                            count0++;
                        }
                        else if (_and0 & 0x1) {
                            if (count0 > max0) max0 = count0;
                            count0 = 1;
                            do {
                                _xor0 = (_xor0 >> 1);
                                _and0 = (_and0 >> 1);
                                z++;
                            } while (_and0 & 0x1);
                            z--;
                            continue;
                        }
                        else {
                            bCounting0 = 0;
                            if (count0 > max0) max0 = count0;
                            count0 = 0;
                        }
                    }
                    else if (_and0 & 0x1) {
                        count0++;
                        bCounting0 = 1;
                        do {
                            _xor0 = (_xor0 >> 1);
                            _and0 = (_and0 >> 1);
                            z++;
                        } while (_and0 & 0x1);
                        z--;
                        continue;
                    }
                    _xor0 = (_xor0 >> 1);
                    _and0 = (_and0 >> 1);
                }
                if (count0 > max0) max0 = count0;
                maxStack0 += max0;

                // C0 is 1
                // w = 0: last bit check is ignored
                //        and C0 area is need to calculate
                for (int w = 0; w < i; w++) {
                    if (bCounting1) {
                        if (_xor1 & 0x1) {
                            count1++;
                        }
                        else if (_and1 & 0x1) {
                            if (count1 > max1) max1 = count1;
                            count1 = 1;
                            do {
                                _xor1 = (_xor1 >> 1);
                                _and1 = (_and1 >> 1);
                                w++;
                            } while (_and1 & 0x1);
                            w--;
                            continue;
                        }
                        else {
                            bCounting1 = 0;
                            if (count1 > max1) max1 = count1;
                            count1 = 0;
                        }
                    }
                    else if (_and1 & 0x1) {
                        count1++;
                        bCounting1 = 1;
                        do {
                            _xor1 = (_xor1 >> 1);
                            _and1 = (_and1 >> 1);
                            w++;
                        } while (_and1 & 0x1);
                        w--;
                        continue;
                    }
                    _xor1 = (_xor1 >> 1);
                    _and1 = (_and1 >> 1);
                }
                if (count1 > max1) max1 = count1;
                maxStack1 += max1;
            }
            if (!(x & 0xff)) printf("\r%d-bit [1/3] %.2lf%% done", i, ((double)x + 1.0) * 100 / (double)limit);
            avgClk0 += (double)maxStack0 / (double)limit;
            avgClk1 += (double)maxStack1 / (double)limit;
        }
        avgClk0 /= (double)limit;
        avgClk1 /= (double)limit;

        // average clock for add 1
        unsigned int maxStack2 = 0;
        double avgClkAdd = 0.0;

        for (unsigned int a = 0; a < limit; a++) {
            int _x = a;
            int count = 0;
            int pos = 1;

            if (_x & 0x1) {
                while ((_x & 0x1) && (pos < i)) {
                    count++;
                    pos++;
                    _x = (_x >> 1);
                }
            }
            maxStack2 += count;
            if (!(a & 0xffffff)) printf("\r%d-bit [2/3] %.2lf%% done", i, ((double)a + 1.0) * 100 / (double)limit);
        }
        avgClkAdd = (double)maxStack2 / (double)limit;

        // average clock for sub 1
        unsigned int maxStack3 = 0;
        double avgClkSub = 0.0;

        for (unsigned int b = 0; b < limit; b++) {
            int _and = b;
            int max = 0;
            int count = 0;
            int bCounting = 0;

            for (int c = 1; c < i; c++) {
                if (bCounting) {
                    if (_and & 0x1) {
                        if (count > max) max = count;
                        count = 1;
                        do {
                            _and = (_and >> 1);
                            c++;
                        } while (_and & 0x1);
                        c--;
                        continue;
                    }
                    else {
                        count++;
                    }
                }
                else if (_and & 0x1) {
                    count = 1;
                    bCounting = 1;
                    do {
                        _and = (_and >> 1);
                        c++;
                    } while (_and & 0x1);
                    c--;
                    continue;
                }
                _and = (_and >> 1);
            }
            if (count > max) max = count;

            maxStack3 += max;
            if (!(b & 0xffffff)) printf("\r%d-bit [3/3] %.2lf%% done", i, ((double)b + 1.0) * 100 / (double)limit);
        }
        avgClkSub = (double)maxStack3 / (double)limit;

        // Final output
        printf("\r%d,%lf,%lf,%lf,%lf,%lf\n", i, avgClk0, avgClk1, (avgClk0 + avgClk1) / 2.0, avgClkAdd, avgClkSub);
    }

    printf("%c\n", _getch());
    while (_kbhit()) dummy = _getch();

    return 0;
}