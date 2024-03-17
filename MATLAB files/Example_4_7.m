clear all;clc
syms c1 c2 c3 c4 c5 c6 s1 s2 s3 s4 s5 s6 d1 d2 d3 d4 d5 d6

T_1_0 = [c1  0 -s1 0; s1 0 c1 0; 0 -1 0 0; 0 0 0 1];
T_2_1 = [c2 0 s2 0; s2 0 -c2 0; 0 1 0 d2; 0 0 0 1];
T_3_2 = [1 0 0 0; 0 1 0 0; 0 0 1 d3; 0 0 0 1];
T_4_3 = [c4 0 -s4 0; s4 0 c4 0; 0 -1 0 0; 0 0 0 1];
T_5_4 = [c5 0 s5 0; s5 0 -c5 0; 0 -1 0 0; 0 0 0 1];
T_6_5 = [c6 -s6 0 0; s6 c6 0 0; 0 0 1 d6; 0 0 0 1];

T_1_0 = T_1_0
T_2_0 = T_1_0 * T_2_1
T_3_0 = T_1_0 * T_2_1 * T_3_2
T_4_0 = T_1_0 * T_2_1 * T_3_2 * T_4_3 
T_5_0 = T_1_0 * T_2_1 * T_3_2 * T_4_3 * T_5_4
T_6_0 = T_1_0 * T_2_1 * T_3_2 * T_4_3 * T_5_4 * T_6_5