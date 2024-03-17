clear all;clc;close all;
image = imread('iTipCropped.jpg');
image = im2bw(image, 0.7);
% image = flipdim(image,2);
% image = imrotate(image,270);
imshow(image)
hold on

line([0 70],[62-6.324 62-49.5],'color','white') %Works with only original
%%
close all;
% Finding Left to Right first white pixel
i=1;
for y = 1:62 %y: Row
    for x = 1:62 %x: Column
        if image(y,x) == 1
            Xl(i) = x;
            Yl(i) = y;
            i = i+1;
            break
        end
    end
end
Yl = 62 - Yl;

figure;
subplot(1,4,1)
plot(Xl,Yl,'.');
title('Left Side of Image')
axis([0 61 0 61])
subplot(1,4,4)
imshow(image)
%%
% Finding Right to Left first white pixel
i=1;
for y = 1:62 %y: Row
    for x = 62:-1:1 %x: Column
        if image(y,x) == 1
            Xr(i) = x;
            Yr(i) = y;
            i = i+1;
            break
        end
    end
end
Yr = 62 - Yr;

subplot(1,4,2)
plot(Xr,Yr,'.');
title('Right Side of Image')
axis([0 61 0 61])

%%
% Finally getting the mid points
X = (Xl+Xr)/2;
Y = (Yl+Yr)/2;
subplot(1,4,3)
plot(X,Y,'.');
title('Skeletonized Image')
axis([0 61 0 61])


figure(2);

subplot(1,3,1)
plot(X,Y,'.');
axis([0 61 0 61])

%%
% Drawing the polynomial
% figure;
% for x = 26:34
%     func = -0.3149*(x^2) + 22.03*x + -357.7;
%     plot(x,func,'r+')
%     hold on;
% end
figure;
syms x
func = -0.3149*(x^2) + 22.03*x + -357.7;
ezplot(func, [26 34]);
hold on;
plot(X,Y,'.');
%%
% Drawing the tangent line
syms x;
func = -0.3149*(x^2) + 22.03*x + (-357.7);
% func = -0.3149*(x^2) + 17.65*x + (-219.5);
ezplot(func, [29 34]);
grid on;
hold on;
d_func = diff(func,x);
syms x1
funcy = (subs(d_func,x,34)*(x1 - 34)) + subs(func,x,34)

% funcy = (subs(d_func,x,32)*(x1 - 32)) + subs(func,x,32)

% ezplot(d_func, [0 70]);
grid on;
h = ezplot(funcy, [26 35])
set(h, 'Color', 'Red')

%%
% Website example
x = [0, 1, 2, 3, 4, 5,  6,  7,  8,  9, 10];
y = [1, 6,17,34,57,86,121,162,209,262,321];
plot(x,y)
hold on
syms X
func = 3*X^2 + 2*X + 1
h = ezplot(func, [0 10])
set(h,'color','red')
%%
% When it works fine
syms P
func = -0.927*P^2    +61.222*P    -984.188
h = ezplot(func,[26 34])
set(h, 'Color', 'Red')
hold on;
y = 0:29;
x = [29 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 28.5 29.5 29.5 30 30 30 30.5 30.5 31.5 31.5 32.5 33 33 32.5 32.5];
plot(x,y,'.')
%%
% When it doesn't work fine
syms P
func = -.170*P^2    +11.83*P    -177.84
ezplot(func,[27 36])
hold on;

hold on;
y = 0:29;
x = [24.5 24 24 24 23.5 23.5 23.5 23.5 23.5 24 24 24 24 24 24.5 24.5 25.5 26.5 27.5 27.5 28.5 29 29.5 30 30.5 31 31.5 32 32.5 32];
plot(x,y,'.')



%% Case : When tip is bend more
syms P
func =      -1.248*P^2         +   79.308*P      -   1231.829
h = ezplot(func,[28 34])
set(h,'Color','Red')
hold on;
funcVal =   -1.248*33*33       +   79.308*33     -   1231.829
dfuncVal =  -1.248*2*33        +   79.308

y1 = dfuncVal * (20 - 33) + funcVal
y2 = dfuncVal * (40 - 33) + funcVal

h = line([20 40],[y1 y2])
set(h,'Color','Green')


%% With average of slopes
syms P
func =      -1.248*P^2         +   79.308*P      -   1231.829
h = ezplot(func,[22 32])
set(h, 'color', 'red')
axis([27 35 20 50])
hold on;
funcVal =   -1.248*33*33       +   79.308*33     -   1231.829
dfuncVal =  -1.248*2*33        +   79.308
y1a = dfuncVal * (20 - 33) + funcVal
y1b = dfuncVal * (40 - 33) + funcVal
slope = dfuncVal;

funcVal =   -1.248*30*30       +   79.308*30     -   1231.829
dfuncVal =  -1.248*2*30        +   79.308
y2a = dfuncVal * (20 - 30) + funcVal
y2b = dfuncVal * (40 - 30) + funcVal
slope = slope + dfuncVal;

funcVal =   -1.248*31*31       +   79.308*31     -   1231.829
dfuncVal =  -1.248*2*31        +   79.308
y3a = dfuncVal * (20 - 31) + funcVal
y3b = dfuncVal * (40 - 31) + funcVal
slope = slope + dfuncVal;

funcVal =   -1.248*32*32       +   79.308*32     -   1231.829
dfuncVal =  -1.248*2*32        +   79.308
y4a = dfuncVal * (20 - 32) + funcVal
y4b = dfuncVal * (40 - 32) + funcVal
slope = slope + dfuncVal;

line([20 40],[y1a y1b])
line([20 40],[y2a y2b])
line([20 40],[y3a y3b])
line([20 40],[y4a y4b])



funcVal =   -1.248*32*32       +   79.308*32     -   1231.829
dfuncVal =  -1.248*2*32        +   79.308
y5a = dfuncVal * (20 - 32) + funcVal
y5b = dfuncVal * (40 - 32) + funcVal

h = line([20 40],[y5a y5b])
set(h,'color','green')
grid on;
% axis([-1000 1000 -1000 1000])