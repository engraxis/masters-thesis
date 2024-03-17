clear all;clc;close all;
image = imread('iTipCropped.jpg');
image = im2bw(image, 0.7);
image = imrotate(image,270);
imshow(image)
hold on

% line([0 70],[62-6.324 62-49.5],'color','white') %Works with only original
% image
%%
close all;
% Finding Left to Right first white pixel
i=1;
for y = 1:62 %y: Row
    for x = 1:62 %x: Column
        if image(x,y) == 1
            Xl(i) = y;
            Yl(i) = x;
            i = i+1;
            break
        end
    end
end
Yl = 62 - Yl;

figure;
subplot(1,4,1)
plot(Xl,Yl,'.');
axis([0 61 0 61])
subplot(1,4,4)
imshow(image)
%%
% Finding Right to Left first white pixel
i=1;
for y = 1:62 %y: Row
    for x = 62:-1:1 %x: Column
        if image(x,y) == 1
            Xr(i) = y;
            Yr(i) = x;
            i = i+1;
            break
        end
    end
end
Yr = 62 - Yr;

subplot(1,4,2)
plot(Xr,Yr,'.');
axis([0 61 0 61])
%%
% Finally getting the mid points
X = (Xl+Xr)/2;
Y = (Yl+Yr)/2;
subplot(1,4,3)
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
func = -0.01534*(x^2) + 0.2094*x + 34.55;
ezplot(func, [0 33]);
hold on;
plot(X,Y,'.');
%%
% Drawing the tangent line
syms x;
func = -0.01534*(x^2) + 0.2094*x + 34.55;
ezplot(func, [0 33]);
grid on;
hold on;
d_func = diff(func,x);

y1a = (subs(d_func,x,30)*(20 - 30)) + subs(func,x,30)
y1b = (subs(d_func,x,30)*(35 - 30)) + subs(func,x,30)
grid on;
h = line([20 35],[y1a y1b])
set(h, 'Color', 'Red')
axis([0 50 0 50])
grid off;
% Now rotating it back 
% Saving image, then rotate it
figure(10)
rotatedimage = imread('Rotated.jpg');
imshow(imrotate(rotatedimage,90));
