%% Validating curve fitting
% Second order polynomial equation y = ax^2 + bx + c

close all;clear all;clc;

X1 = [34  34  34  33  33  32  31.5  31  30.5  30   29  29   28.5  28.5  28  27.5  27.5  27.5  27  27  27  26.5  26.5  26.5  26.5  26.5  27  27  27.5  27.5];
Y1 = [29  28  27  26  25  24    23  22    21  20   19  18     17    16  15    14    13    12  11  10   9     8     7     6     5     4   3   2     1     0];

X2 = [34  34  34  33  33  32  31.5  31  30.5       29  29   28.5  28.5  28  27.5  27.5  27.5  27  27  27  26.5  26.5  26.5  26.5  26.5  27  27  27.5  27.5];
Y2 = [29  28  30  26  28  24    23  22    21       19  18     20    16  15    14    13    14  11  10   9     8     7     6     5     4   3   2     1     0];

X3 = [34  34  34  33  33  32  31.5  31        30   29  29   28.5  28.5  28  27.5        27.5  27  27  27  26.5  26.5  26.5  26.5        27  27  27.5  27.5];
Y3 = [29  28  27  26  25  24    23  22        20   19  18     17    16  15    14          12  11  10   9     8     7     6     5         3   2     1     0];

a1 = -0.3149;
b1 = 22.03;
c1 = -357.7;

a2 = -0.3035;
b2 = 21.46;
c2 = -350.5;

a3 = -0.3061;
b3 = 21.51;
c3 = -350.2;

index = 1;
for xx = 26: .1 : 34
    yy1(index) = a1*(xx^2) + b1*xx + c1;
    yy2(index) = a2*(xx^2) + b2*xx + c2;
    yy3(index) = a3*(xx^2) + b3*xx + c3;
    xx123(index) = xx;
    index = index + 1;
end

plot(X1,Y1,'r*');
figure;
plot(X2,Y2,'g*');
figure;
plot(X3,Y3,'b*');
figure;
plot(xx123,yy1, 'r')
hold on
plot(xx123,yy2, 'g')
plot(xx123,yy3, 'b')
legend('Orignial Image', 'Changed Image 1', 'Changed Image 2')

% Tangent Slope
SlopeAtPoint = 34;
FunctionValue1 = a1*SlopeAtPoint*SlopeAtPoint + b1*SlopeAtPoint + c1;
FunctionValue2 = a2*SlopeAtPoint*SlopeAtPoint + b2*SlopeAtPoint + c2;
FunctionValue3 = a3*SlopeAtPoint*SlopeAtPoint + b3*SlopeAtPoint + c3;

TangentSlope1 = 2*a1*SlopeAtPoint + b1;
TangentSlope2 = 2*a2*SlopeAtPoint + b2;
TangentSlope3 = 2*a3*SlopeAtPoint + b3;

% Drawing Tangent Lines
LineX1 = 20;
LineX2 = 40;

Line1Y1 = TangentSlope1 * (LineX1 - SlopeAtPoint) + FunctionValue1;
Line1Y2 = TangentSlope1 * (LineX2 - SlopeAtPoint) + FunctionValue1;


Line2Y1 = TangentSlope2 * (LineX1 - SlopeAtPoint) + FunctionValue2;
Line2Y2 = TangentSlope2 * (LineX2 - SlopeAtPoint) + FunctionValue2;

Line3Y1 = TangentSlope3 * (LineX1 - SlopeAtPoint) + FunctionValue3;
Line3Y2 = TangentSlope3 * (LineX2 - SlopeAtPoint) + FunctionValue3;

hold on;
Tangent1 = line([LineX1 LineX2],[Line1Y1 Line1Y2]);
Tangent2 = line([LineX1 LineX2],[Line2Y1 Line2Y2]);
Tangent3 = line([LineX1 LineX2],[Line3Y1 Line3Y2]);
set(Tangent1, 'color', 'r')
set(Tangent2, 'color', 'g')
set(Tangent3, 'color', 'b')

% Tangent Angle
TangentAngle1x = 40 - 20;
TangentAngle1y = Line1Y2 - Line1Y1; 
TangentAngle1  = atan2(TangentAngle1y, TangentAngle1x);
disp('Angle of Tangent for data1');
TangentAngle1 = TangentAngle1 * 180 / pi

TangentAngle2x = 40 - 20;
TangentAngle2y = Line2Y2 - Line2Y1;
TangentAngle2  = atan2(TangentAngle2y, TangentAngle2x);
disp('Angle of Tangent for data2');
TangentAngle2 = TangentAngle2 * 180/ pi

TangentAngle3x = 40 - 20;
TangentAngle3y = Line3Y2 - Line3Y1;
TangentAngle3  = atan2(TangentAngle3y, TangentAngle3x);
disp('Angle of Tangent for data3');
TangentAngle3 = TangentAngle3 * 180/ pi