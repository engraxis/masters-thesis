close all;
clear all;
clc;

%% Problem 5-19
q0 = 10;
qf = 120;

index = 1;

for t = 0:0.1:2
    pos(index) = q0 + (((3*qf - 3*q0 - 2)/4) * t^2) + (((q0 - qf + 1)/4) * t^3);
    index = index + 1;
end

t = 0:0.1:2;
plot(t, pos)

%% Problem 5-20
q0 = 10;
qf = 120;
V = 10;

tb = (q0 - qf + (2*V))/V;
