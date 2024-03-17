clear all;clc

syms a b t;

Rza =  [ cos(a) -sin(a)         0;  sin(a)  cos(a) 0;       0  0       1];
Ryb =  [ cos(b)       0    sin(b);       0       1 0; -sin(b)  0  cos(b)];
Rzth = [ cos(t) -sin(t)         0;  sin(t)  cos(t) 0;       0  0       1];
Ry_b = [ cos(b)       0   -sin(b);       0       1 0;  sin(b)  0  cos(b)];
Rz_a = [ cos(a)  sin(a)         0; -sin(a)  cos(a) 0;       0  0       1];

R = Rza * Ryb * Rzth * Ry_b * Rz_a;

syms kx ky kz
sin_a = ky/sqrt(kx^2 + ky^2);
cos_a = kx/sqrt(kx^2 + ky^2);
sin_b = sqrt(kx^2 + ky^2);
cos_b = kz;

R_subs = subs(R, {sin(a), cos(a), sin(b), cos(b)}, {sin_a, cos_a, sin_b, cos_b});

syms W;

R_k_theta = subs(R_subs, (kx^2 + ky^2)^(1/2), W)