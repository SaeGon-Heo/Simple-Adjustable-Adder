# Simple-Adjustable-Adder(SAA)
This is an adder(a logic circuit) which has simple structure and adjustable about bits.

# 64-bit SAA Structure
![adder](https://user-images.githubusercontent.com/46312473/72669309-92fe2280-3a73-11ea-9bc5-80148b240e62.PNG)
![adder2](https://user-images.githubusercontent.com/46312473/72669311-985b6d00-3a73-11ea-824f-c772b5c7f6cb.PNG)
![adder3](https://user-images.githubusercontent.com/46312473/72669313-9db8b780-3a73-11ea-8250-0e726e41c22e.PNG)
Tool: Max+plus II 10.2(Student license)  
There are no device for this design with student license as I remember.  
But I'm sure I tested this with 16-bit version and it worked.  
So this must be no problem.  

# Basics
For example, we can add A and B with C0 using a 8-bit SAA.  

> A: 1100 0111 / B: 0101 0001 / C0: 1  

Add process is here.
1. Calculate and put each SUM and CARRY from bit's position.  
   And store C0.  
> A+B+C0: \[7-4\](01)(10)(00)(01) \[3-0\](00)(01)(01)(10) \[C0\](1?)  
> Each bit has 2 bits storage show as (XY)  
> X: CARRY, Y: SUM  

2. Calculate and put current bit's SUM and current bit's CARRY for all bits  
   using current bit's SUM and previous bit's CARRY.  
> clock 0: \[C8\](?0) \[7-4\](01)(10)(00)(01) \[3-0\](00)(01)(01)(10) \[C0\](1?)  
> clock 1: \[C8\](?0) \[7-4\](10)(00)(00)(01) \[3-0\](00)(01)(10)(01) \[C0\](0?)  

3. Repeat \[2.\] until each bit's CARRY except highest bit's CARRY(7) become 0.  
> clock 1: \[C8\](?0) \[7-4\](10)(00)(00)(01) \[3-0\](00)(01)(10)(01) \[C0\](0?)  
> clock 2: \[C8\](?1) \[7-4\](00)(00)(00)(01) \[3-0\](00)(10)(00)(01) \[C0\](0?)  
> clock 3: \[C8\](?1) \[7-4\](00)(00)(00)(01) \[3-0\](01)(00)(00)(01) \[C0\](0?)  

4. All CARRY are calculated __in parallel__ which don't care position.  
   And output is each bit's SUM.
> Output is calculated for 3 clocks.  
> From \[C8\](?1) \[7-4\](00)(00)(00)(01) \[3-0\](01)(00)(00)(01) \[C0\](0?)  
> Ans: 0001 1001 / C8: 1  

![calc](https://user-images.githubusercontent.com/46312473/72671550-8b4c7700-3a8f-11ea-8ed5-b1d3cfa6c810.png)

# How it Works
> Sorry I can't tell how this work well. Please, try find how this work.  
> Not much hard to understand I believe.  
> Any good instruction for this is welcome. PR would be my pleasure.  

![base](https://user-images.githubusercontent.com/46312473/72670793-3ad01c00-3a85-11ea-96de-085fc387dc05.PNG)
Each bit calculation need 2 Flip-Flop for SUM and CARRY.  

0. Clear all Flip-Flop to 0 by putting 0 on CLR pin for 1 clock.  
   After 1 clock, input A, B and C0 by putting 1 on LOAD pin for 1 clock.  
1. Initial A0 and B0's SUM and CARRY is calculated and stored to center and below Flip-Flop.  
   C0 also stored to upper Flip-Flop.  
2. After 1 clock, 0'bit SUM and CARRY are calculated using an upper(C0) and center(0'bit SUM) Flip-Flop's Q.   
   And put them to center(0'bit SUM = Q0) and below(0'bit CARRY = COut1) Flip-Flop's Q.  
   By this C0 has been calculated and become 0 because upper Flip-Flip's D is 0 now(LOAD is 0).  
3. When all carry except highest carry become 0, FIN pin become 1 and meaning calculation is done.  

And this adder is extendable very easily by repeating 0'bit area.  
Like # 64-bit SAA Structure, but more bits size, too.  

# Performance
- Input clock  
All case: 1 clock (or maybe 2 clocks)  
- Carry calculation clock  
Best case: 0 clocks  
Worst case: n clocks (n = bits)  

One clock time must bigger than "max(T(XOR2), T(AND2)) + T(D Flip-Flop) + T(OR2)".  

**Average clock for carry calculation**

bits|C0 = 0|C0 = 1|All case|Add 1|Sub 1 
---- | ---- | ---- | ---- | ---- | ----  
2|0.250000|1.500000|0.875000|0.500000|0.500000
3|0.562500|1.875000|1.218750|0.750000|1.000000
4|0.890625|2.125000|1.507813|0.875000|1.500000
5|1.199219|2.328125|1.763672|0.937500|1.937500
6|1.481445|2.492188|1.986816|0.968750|2.312500
7|1.734131|2.638672|2.186401|0.984375|2.625000
8|1.960266|2.768555|2.364410|0.992188|2.890625
9|2.162338|2.888428|2.525383|0.996094|3.117188
10|2.343861|2.999146|2.671503|0.998047|3.314453
11|2.507682|3.102692|2.805187|0.999023|3.488281
12|2.656434|3.199814|2.928124|0.999512|3.643555
13|2.792279|3.291393|3.041836|0.999756|3.783691
14|2.917058|3.377935|3.147497|0.999878|3.911377
15|3.032277|3.459979|3.246128|0.999939|4.028564
16|3.139202|3.537919|3.338561|0.999969|4.136841
17|3.238882|3.612126|3.425504|0.999985|4.237442
18|3.332193|3.682915|3.507554|0.999992|4.331375
19|3.419873|3.750568|3.585221|0.999996|4.419464
20|||||
21|||||
22|||||
23|||||
24|||||
25|||||
26|||||
27|||||

**Plot**

![plot](https://user-images.githubusercontent.com/46312473/73046172-5f2c5e00-3eb4-11ea-8b64-ffe80cf47a7e.PNG)

> A maximum bits is 27 in [Average clock calculation code](https://github.com/SaeGon-Heo/Simple-Adjustable-Adder/blob/master/SAA-avg-clock-calculator.c).  
> But after 19 bits, the calculation time is too big for my i5-3570 cpu.  
> PR would be my pleasure if anyone using a good cpu fill in the above table.  

# License
This project under a [BSD 2-Clause "Simplified" License](https://github.com/SaeGon-Heo/Simple-Adjustable-Adder/blob/master/LICENSE).

# Note
Because I have only .gdf and screenshots now, I can't make any change from here.  
If you want to edit this, you have to use "Max+plus II 10.2" and get a license from Altera web site or the University.  
Or you may draw a SAA using the other tools.  
