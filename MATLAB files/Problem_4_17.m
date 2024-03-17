clear all;clc

syms c1 c2 c3 s1 s2 s3 d1 a2 a3
T_1_0 = [c1 0 s1 0; s1 0 -c1 0; 0 1 0 d1; 0 0 0 1]
T_2_1 = [c2 -s2 0 a2*c2; s2 c2 0 a2*s2; 0 0 1 0; 0 0 0 1 ]
T_3_2 = [c3 -s3 0 a3*c3 ; s3 c3 0 a3*s3; 0 0 1 0; 0 0 0 1]

T_1_0 = T_1_0
T_2_0 = T_1_0 * T_2_1
T_3_0 = T_1_0 * T_2_1 * T_3_2