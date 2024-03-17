close all;clear all;clc;
x = [10:0.08:80];
y = cosd(x);

figure(1);
subplot(1,3,1);
plot(x, y);
xlabel('Angle (deg)');
ylabel('Cosine of Angle');
title('Cosine Curve');
axis([10 80 0 6])

subplot(1,3,2);
yval = 1 - y;

plot(x, yval);
xlabel('Angle (deg)');
ylabel('Gain');
title('Gain Curve');
axis([10 80 0 6])

subplot(1,3,3);
gain = yval * 5;
plot(x, gain);
xlabel('Angle (deg)');
ylabel('Torque (Nmm)');
title('Torque Curve');
axis([10 80 0 6])