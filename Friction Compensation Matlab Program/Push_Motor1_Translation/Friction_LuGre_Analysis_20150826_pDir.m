% Knee Friction compensator redesign
% 20140926

% Positive Direction

clear all;
close all;
clc

% 0 ~ 1.917 [rad/s] Link 속도 범위에서 마찰 측정
% PI 속도 제어를 통해서 등속도 구간의 데이터를 저장

% velocit resolution : 5.1458764 mm/s

% DATA Load
DATA1 = [load('SaveData1.dat')];
DATA2 = [load('SaveData2.dat')];
DATA3 = [load('SaveData3.dat')];
DATA4 = [load('SaveData4.dat')];
DATA5 = [load('SaveData5.dat')];
DATA6 = [load('SaveData6.dat')];
DATA7 = [load('SaveData7.dat')];
DATA8 = [load('SaveData8.dat')];
DATA9 = [load('SaveData9.dat')];
DATA10 = [load('SaveData10.dat')];
DATA11 = [load('SaveData11.dat')];
DATA12 = [load('SaveData12.dat')];
DATA13 = [load('SaveData13.dat')];
DATA14 = [load('SaveData14.dat')];
DATA15 = [load('SaveData15.dat')];
DATA16 = [load('SaveData16.dat')];
DATA17 = [load('SaveData17.dat')];
DATA18 = [load('SaveData18.dat')];
DATA19 = [load('SaveData19.dat')];
DATA20 = [load('SaveData20.dat')];
DATA21 = [load('SaveData21.dat')];
DATA22 = [load('SaveData22.dat')];
DATA23 = [load('SaveData23.dat')];
DATA24 = [load('SaveData24.dat')];
DATA25 = [load('SaveData25.dat')];
DATA26 = [load('SaveData26.dat')];
DATA27 = [load('SaveData27.dat')];
DATA28 = [load('SaveData28.dat')];
DATA29 = [load('SaveData29.dat')];
DATA30 = [load('SaveData30.dat')];

%% 저장 인덱스에 + 2하면 됨.
figure(1)
% Error
plot(DATA30(1:2000,11))
title('Error [mm/s]')

figure(2)
plot(DATA30(1160:1380,11))

%%
% 1번: 2000:8000
V1 = DATA1(2000:8000, 5);
F1 = DATA1(2000:8000, 9);
Verr1 = DATA1(2000:8000, 11);

ind = find(DATA1(2000:8000, 11) == 0);

VF1 = [V1, F1];

figure(3)
stem(V1, 'LineStyle', 'none')
figure(4)
%stem(F1, 'LineStyle', 'none')
stem(F1(ind), 'LineStyle', 'none')

figure(5)
stem(Verr1(ind), 'LineStyle', 'none')
%stem(DATA1(2000:8000, 3), F1, 'LineStyle', 'none')

% Data conditioning (이상치 등 이상한 값 제거)
Z = zscore(VF1(:,1));
outlier = find(abs(Z) > 3)

pVF1 = VF1;

        
%%    
% 2번: 1000:5000
V2 = DATA2(1000:5000, 5);
F2 = DATA2(1000:5000, 9);

VF2 = [V2, F2];

figure(3)
stem(V2, 'LineStyle', 'none')
figure(4)
stem(F2, 'LineStyle', 'none')

figure(5)
stem(DATA2(1000:3000, 3), F2, 'LineStyle', 'none')

%
Z = zscore(VF2(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF2(:,1);
temp_V(outlier) = [];

temp_F = VF2(:,2);
temp_F(outlier) = [];
        
pVF2 = [temp_V, temp_F];


%%
% 3: 1000:3500
V3 = DATA3(1000:3500, 5);
F3 = DATA3(1000:3500, 9);

VF3 = [V3, F3];

figure(3)
stem(V3, 'LineStyle', 'none')
figure(4)
stem(F3, 'LineStyle', 'none')

%
Z = zscore(VF3(:,1));
outlier = find(abs(Z) > 3)

pVF3 = VF3;


%%
% 4: 1000:3000
V4 = DATA4(1000:3000, 5);
F4 = DATA4(1000:3000, 9);

VF4 = [V4, F4];

figure(3)
stem(V4, 'LineStyle', 'none')
figure(4)
stem(F4, 'LineStyle', 'none')

figure(5)
stem(DATA4(1000:3000, 3), F4, 'LineStyle', 'none')

%
Z = zscore(VF4(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF4(:,1);
temp_V(outlier) = [];

temp_F = VF4(:,2);
temp_F(outlier) = [];
        
pVF4 = [temp_V, temp_F];
%%
% 5: 1000:2500
V5 = DATA5(1000:2500, 5);
F5 = DATA5(1000:2500, 9);

VF5 = [V5, F5];

figure(3)
stem(V5, 'LineStyle', 'none')
figure(4)
stem(F5, 'LineStyle', 'none')

%
Z = zscore(VF5(:,1));
outlier = find(abs(Z) > 3)

pVF5 = VF5;

%%
% 6: 1000:2000
V6 = DATA6(1000:2000, 5);
F6 = DATA6(1000:2000, 9);

VF6 = [V6, F6];

figure(3)
stem(V6, 'LineStyle', 'none')
figure(4)
stem(F6, 'LineStyle', 'none')

%
Z = zscore(VF6(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF6(:,1);
temp_V(outlier) = [];

temp_F = VF6(:,2);
temp_F(outlier) = [];
        
pVF6 = [temp_V, temp_F];

%%
% 7: 800:1800
V7 = DATA7(800:1800, 5);
F7 = DATA7(800:1800, 9);

VF7 = [V7, F7];

figure(3)
stem(V7, 'LineStyle', 'none')
figure(4)
stem(F7, 'LineStyle', 'none')

%
Z = zscore(VF7(:,1));
outlier = find(abs(Z) > 3)

pVF7 = VF7;

%%
% 8: 1300:2300
V8 = DATA8(1300:2300, 5);
F8 = DATA8(1300:2300, 9);

VF8 = [V8, F8];

figure(3)
stem(V8, 'LineStyle', 'none')
figure(4)
stem(F8, 'LineStyle', 'none')

%
Z = zscore(VF8(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF8(:,1);
temp_V(outlier) = [];

temp_F = VF8(:,2);
temp_F(outlier) = [];
        
pVF8 = [temp_V, temp_F];
%%
% 9: 1100:2000
V9 = DATA9(1100:2000, 5);
F9 = DATA9(1100:2000, 9);

VF9 = [V9, F9];

figure(3)
stem(V9, 'LineStyle', 'none')
figure(4)
stem(F9, 'LineStyle', 'none')

%
Z = zscore(VF9(:,1));
outlier = find(abs(Z) > 3)

pVF9 = VF9;
%%
% 10: 1300:2100
V10 = DATA10(1300:2100, 5);
F10 = DATA10(1300:2100, 9);

VF10 = [V10, F10];

figure(3)
stem(V10, 'LineStyle', 'none')
figure(4)
stem(F10, 'LineStyle', 'none')
%
Z = zscore(VF10(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF10(:,1);
temp_V(outlier) = [];

temp_F = VF10(:,2);
temp_F(outlier) = [];
        
pVF10 = [temp_V, temp_F];
%%
% 11: 1200:1900
V11 = DATA11(1200:1900, 5);
F11 = DATA11(1200:1900, 9);

VF11 = [V11, F11];

figure(3)
stem(V11, 'LineStyle', 'none')
figure(4)
stem(F11, 'LineStyle', 'none')

%
Z = zscore(VF11(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF11(:,1);
temp_V(outlier) = [];

temp_F = VF11(:,2);
temp_F(outlier) = [];
        
pVF11 = [temp_V, temp_F];
%%
% 12: 1150:1800
V12 = DATA12(1150:1800, 5);
F12 = DATA12(1150:1800, 9);

VF12 = [V12, F12];

figure(3)
stem(V12, 'LineStyle', 'none')
figure(4)
stem(F12, 'LineStyle', 'none')

%
Z = zscore(VF12(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF12(:,1);
temp_V(outlier) = [];

temp_F = VF12(:,2);
temp_F(outlier) = [];
        
pVF12 = [temp_V, temp_F];
%%
% 13: 1300:1900
V13 = DATA13(1300:1900, 5);
F13 = DATA13(1300:1900, 9);

VF13 = [V13, F13];

figure(3)
stem(V13, 'LineStyle', 'none')
figure(4)
stem(F13, 'LineStyle', 'none')

%
Z = zscore(VF13(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF13(:,1);
temp_V(outlier) = [];

temp_F = VF13(:,2);
temp_F(outlier) = [];
        
pVF13 = [temp_V, temp_F];
%%
% 14: 1050:1600
V14 = DATA14(1050:1600, 5);
F14 = DATA14(1050:1600, 9);

VF14 = [V14, F14];

figure(3)
stem(V14, 'LineStyle', 'none')
figure(4)
stem(F14, 'LineStyle', 'none')

%
Z = zscore(VF14(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF14(:,1);
temp_V(outlier) = [];

temp_F = VF14(:,2);
temp_F(outlier) = [];
        
pVF14 = [temp_V, temp_F];
%%
% 15: 1000:1550
V15 = DATA15(1000:1550, 5);
F15 = DATA15(1000:1550, 9);

VF15 = [V15, F15];

figure(3)
stem(V15, 'LineStyle', 'none')
figure(4)
stem(F15, 'LineStyle', 'none')

%
Z = zscore(VF15(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF15(:,1);
temp_V(outlier) = [];

temp_F = VF15(:,2);
temp_F(outlier) = [];
        
pVF15 = [temp_V, temp_F];
%%
% 16: 1150:1600
V16 = DATA16(1150:1600, 5);
F16 = DATA16(1150:1600, 9);

VF16 = [V16, F16];

figure(3)
stem(V16, 'LineStyle', 'none')
figure(4)
stem(F16, 'LineStyle', 'none')

%
Z = zscore(VF16(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF16(:,1);
temp_V(outlier) = [];

temp_F = VF16(:,2);
temp_F(outlier) = [];
        
pVF16 = [temp_V, temp_F];
%%
% 17: 750:1200
V17 = DATA17(750:1200, 5);
F17 = DATA17(750:1200, 9);

VF17 = [V17, F17];

figure(3)
stem(V17, 'LineStyle', 'none')
figure(4)
stem(F17, 'LineStyle', 'none')

%
Z = zscore(VF17(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF17(:,1);
temp_V(outlier) = [];

temp_F = VF17(:,2);
temp_F(outlier) = [];
        
pVF17 = [temp_V, temp_F];
%%
% 18: 1500:1900
V18 = DATA18(1500:1900, 5);
F18 = DATA18(1500:1900, 9);

VF18 = [V18, F18];

figure(3)
stem(V18, 'LineStyle', 'none')
figure(4)
stem(F18, 'LineStyle', 'none')

%
Z = zscore(VF18(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF18(:,1);
temp_V(outlier) = [];

temp_F = VF18(:,2);
temp_F(outlier) = [];
        
pVF18 = [temp_V, temp_F];
%%
% 19: 1250:1600
V19 = DATA19(1250:1600, 5);
F19 = DATA19(1250:1600, 9);

VF19 = [V19, F19];

figure(3)
stem(V19, 'LineStyle', 'none')
figure(4)
stem(F19, 'LineStyle', 'none')

%
Z = zscore(VF19(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF19(:,1);
temp_V(outlier) = [];

temp_F = VF19(:,2);
temp_F(outlier) = [];
        
pVF19 = [temp_V, temp_F];
%%
% 20: 1300:1650
V20 = DATA20(1300:1650, 5);
F20 = DATA20(1300:1650, 9);

VF20 = [V20, F20];

figure(3)
stem(V20, 'LineStyle', 'none')
figure(4)
stem(F20, 'LineStyle', 'none')

%
Z = zscore(VF20(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF20(:,1);
temp_V(outlier) = [];

temp_F = VF20(:,2);
temp_F(outlier) = [];
        
pVF20 = [temp_V, temp_F];
%%
% 21: 1250:1600
V21 = DATA21(1250:1600, 5);
F21 = DATA21(1250:1600, 9);

VF21 = [V21, F21];

figure(3)
stem(V21, 'LineStyle', 'none')
figure(4)
stem(F21, 'LineStyle', 'none')

%
Z = zscore(VF21(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF21(:,1);
temp_V(outlier) = [];

temp_F = VF21(:,2);
temp_F(outlier) = [];
        
pVF21 = [temp_V, temp_F];
%%
% 22: 1100:1450
V22 = DATA22(1100:1450, 5);
F22 = DATA22(1100:1450, 9);

VF22 = [V22, F22];

figure(3)
stem(V22, 'LineStyle', 'none')
figure(4)
stem(F22, 'LineStyle', 'none')

%
Z = zscore(VF22(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF22(:,1);
temp_V(outlier) = [];

temp_F = VF22(:,2);
temp_F(outlier) = [];
        
pVF22 = [temp_V, temp_F];
%%
% 23: 1100:1400
V23 = DATA23(1100:1400, 5);
F23 = DATA23(1100:1400, 9);

VF23 = [V23, F23];

figure(3)
stem(V23, 'LineStyle', 'none')
figure(4)
stem(F23, 'LineStyle', 'none')

%
Z = zscore(VF23(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF23(:,1);
temp_V(outlier) = [];

temp_F = VF23(:,2);
temp_F(outlier) = [];
        
pVF23 = [temp_V, temp_F];
%%
% 24: 1620:1920
V24 = DATA24(1620:1920, 5);
F24 = DATA24(1620:1920, 9);

VF24 = [V24, F24];

figure(3)
stem(V24, 'LineStyle', 'none')
figure(4)
stem(F24, 'LineStyle', 'none')

%
Z = zscore(VF24(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF24(:,1);
temp_V(outlier) = [];

temp_F = VF24(:,2);
temp_F(outlier) = [];
        
pVF24 = [temp_V, temp_F];
%%
% 25: 1520:1800
V25 = DATA25(1520:1800, 5);
F25 = DATA25(1520:1800, 9);

VF25 = [V25, F25];

figure(3)
stem(V25, 'LineStyle', 'none')
figure(4)
stem(F25, 'LineStyle', 'none')

%
Z = zscore(VF25(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF25(:,1);
temp_V(outlier) = [];

temp_F = VF25(:,2);
temp_F(outlier) = [];
        
pVF25 = [temp_V, temp_F];
%%
% 26: 1320:1600
V26 = DATA26(1320:1600, 5);
F26 = DATA26(1320:1600, 9);

VF26 = [V26, F26];

figure(3)
stem(V26, 'LineStyle', 'none')
figure(4)
stem(F26, 'LineStyle', 'none')

%
Z = zscore(VF26(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF26(:,1);
temp_V(outlier) = [];

temp_F = VF26(:,2);
temp_F(outlier) = [];
        
pVF26 = [temp_V, temp_F];
%%
% 27: 1400:1700
V27 = DATA27(1400:1700, 5);
F27 = DATA27(1400:1700, 9);

VF27 = [V27, F27];

figure(3)
stem(V27, 'LineStyle', 'none')
figure(4)
stem(F27, 'LineStyle', 'none')

%
Z = zscore(VF27(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF27(:,1);
temp_V(outlier) = [];

temp_F = VF27(:,2);
temp_F(outlier) = [];
        
pVF27 = [temp_V, temp_F];
%%
% 28: 1200:1450
V28 = DATA28(1200:1450, 5);
F28 = DATA28(1200:1450, 9);

VF28 = [V28, F28];

figure(3)
stem(V28, 'LineStyle', 'none')
figure(4)
stem(F28, 'LineStyle', 'none')

%
Z = zscore(VF28(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF28(:,1);
temp_V(outlier) = [];

temp_F = VF28(:,2);
temp_F(outlier) = [];
        
pVF28 = [temp_V, temp_F];
%%
% 29: 1600:1840
V29 = DATA29(1600:1840, 5);
F29 = DATA29(1600:1840, 9);

VF29 = [V29, F29];

figure(3)
stem(V29, 'LineStyle', 'none')
figure(4)
stem(F29, 'LineStyle', 'none')

%
Z = zscore(VF29(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF29(:,1);
temp_V(outlier) = [];

temp_F = VF29(:,2);
temp_F(outlier) = [];
        
pVF29 = [temp_V, temp_F];
%%
% 30: 1160:1380
V30 = DATA30(1160:1380, 5);
F30 = DATA30(1160:1380, 9);

VF30 = [V30, F30];

figure(3)
stem(V30, 'LineStyle', 'none')
figure(4)
stem(F30, 'LineStyle', 'none')

%
Z = zscore(VF30(:,1));
outlier = find(abs(Z) > 3)

temp_V = VF30(:,1);
temp_V(outlier) = [];

temp_F = VF30(:,2);
temp_F(outlier) = [];
        
pVF30 = [temp_V, temp_F];

%%
% 속도
figure(3)
%plot(DATA1(2000:8000,5))
stem(DATA1(2000:8000,5), 'LineStyle', 'none')
title('Vel [mm/s]')

% 힘
figure(4)
stem(DATA1(2000:8000,9), 'LineStyle', 'none')
title('Force [N]')


%%
% plot measured shaft velocity vs. torque control input
% 통합된 Velocity vs. measured Torque graph
figure(5)
plot(pVF1(:,1), pVF1(:,2), '.')
hold on;
plot(pVF2(:,1), pVF2(:,2), '.')
plot(pVF3(:,1), pVF3(:,2), '.')
plot(pVF4(:,1), pVF4(:,2), '.')
plot(pVF5(:,1), pVF5(:,2), '.')
plot(pVF6(:,1), pVF6(:,2), '.')
plot(pVF7(:,1), pVF7(:,2), '.')
plot(pVF8(:,1), pVF8(:,2), '.')
plot(pVF9(:,1), pVF9(:,2), '.')
plot(pVF10(:,1), pVF10(:,2), '.')
plot(pVF11(:,1), pVF11(:,2), '.')
plot(pVF12(:,1), pVF12(:,2), '.')
plot(pVF13(:,1), pVF13(:,2), '.')
plot(pVF14(:,1), pVF14(:,2), '.')
plot(pVF15(:,1), pVF15(:,2), '.')
plot(pVF16(:,1), pVF16(:,2), '.')
plot(pVF17(:,1), pVF17(:,2), '.')
plot(pVF18(:,1), pVF18(:,2), '.')
plot(pVF19(:,1), pVF19(:,2), '.')
plot(pVF20(:,1), pVF20(:,2), '.')
plot(pVF21(:,1), pVF21(:,2), '.')
plot(pVF22(:,1), pVF22(:,2), '.')
plot(pVF23(:,1), pVF23(:,2), '.')
plot(pVF24(:,1), pVF24(:,2), '.')
plot(pVF25(:,1), pVF25(:,2), '.')
plot(pVF26(:,1), pVF26(:,2), '.')
plot(pVF27(:,1), pVF27(:,2), '.')
plot(pVF28(:,1), pVF28(:,2), '.')
plot(pVF29(:,1), pVF29(:,2), '.')
plot(pVF30(:,1), pVF30(:,2), '.')
hold off;
xlabel('velocity [mm/s]')
ylabel('Force [N]')
title('Velocity vs. Friction')


%% mean value
mVF1 = [mean(pVF1(:,1)), mean(pVF1(:,2))];
mVF2 = [mean(pVF2(:,1)), mean(pVF2(:,2))];
mVF3 = [mean(pVF3(:,1)), mean(pVF3(:,2))];
mVF4 = [mean(pVF4(:,1)), mean(pVF4(:,2))];
mVF5 = [mean(pVF5(:,1)), mean(pVF5(:,2))];
mVF6 = [mean(pVF6(:,1)), mean(pVF6(:,2))];
mVF7 = [mean(pVF7(:,1)), mean(pVF7(:,2))];
mVF8 = [mean(pVF8(:,1)), mean(pVF8(:,2))];
mVF9 = [mean(pVF9(:,1)), mean(pVF9(:,2))];
mVF10 = [mean(pVF10(:,1)), mean(pVF10(:,2))];
mVF11 = [mean(pVF11(:,1)), mean(pVF11(:,2))];
mVF12 = [mean(pVF12(:,1)), mean(pVF12(:,2))];
mVF13 = [mean(pVF13(:,1)), mean(pVF13(:,2))];
mVF14 = [mean(pVF14(:,1)), mean(pVF14(:,2))];
mVF15 = [mean(pVF15(:,1)), mean(pVF15(:,2))];
mVF16 = [mean(pVF16(:,1)), mean(pVF16(:,2))];
mVF17 = [mean(pVF17(:,1)), mean(pVF17(:,2))];
mVF18 = [mean(pVF18(:,1)), mean(pVF18(:,2))];
mVF19 = [mean(pVF19(:,1)), mean(pVF19(:,2))];
mVF20 = [mean(pVF20(:,1)), mean(pVF20(:,2))];
mVF21 = [mean(pVF21(:,1)), mean(pVF21(:,2))];
mVF22 = [mean(pVF22(:,1)), mean(pVF22(:,2))];
mVF23 = [mean(pVF23(:,1)), mean(pVF23(:,2))];
mVF24 = [mean(pVF24(:,1)), mean(pVF24(:,2))];
mVF25 = [mean(pVF25(:,1)), mean(pVF25(:,2))];
mVF26 = [mean(pVF26(:,1)), mean(pVF26(:,2))];
mVF27 = [mean(pVF27(:,1)), mean(pVF27(:,2))];
mVF28 = [mean(pVF28(:,1)), mean(pVF28(:,2))];
mVF29 = [mean(pVF29(:,1)), mean(pVF29(:,2))];
mVF30 = [mean(pVF30(:,1)), mean(pVF30(:,2))];


N = 30;

% velocity data
pV = [mVF1(:,1), mVF2(:,1), mVF3(:,1), mVF4(:,1), mVF5(:,1), ...
    mVF6(:,1), mVF7(:,1), mVF8(:,1), mVF9(:,1), mVF10(:,1), ...
    mVF11(:,1), mVF12(:,1), mVF13(:,1), mVF14(:,1), mVF15(:,1), ...
    mVF16(:,1), mVF17(:,1), mVF18(:,1), mVF19(:,1), mVF20(:,1), ...
    mVF21(:,1), mVF22(:,1), mVF23(:,1), mVF24(:,1), mVF25(:,1), ...
    mVF26(:,1), mVF27(:,1), mVF28(:,1), mVF29(:,1), mVF30(:,1)];

% Force data (Friction)
pF = [mVF1(:,2), mVF2(:,2), mVF3(:,2), mVF4(:,2), mVF5(:,2), ...
    mVF6(:,2), mVF7(:,2), mVF8(:,2), mVF9(:,2), mVF10(:,2), ...
    mVF11(:,2), mVF12(:,2), mVF13(:,2), mVF14(:,2), mVF15(:,2), ...
    mVF16(:,2), mVF17(:,2), mVF18(:,2), mVF19(:,2), mVF20(:,2), ...
    mVF21(:,2), mVF22(:,2), mVF23(:,2), mVF24(:,2), mVF25(:,2), ...
    mVF26(:,2), mVF27(:,2), mVF28(:,2), mVF29(:,2), mVF30(:,2)];

%

figure(6)
plot(pV, pF, '.')

xlabel('velocity [mm/s]')
ylabel('Force [N]')
title('mean Velocity vs. mean Friction')




% 
% 
% %% Comparison raw mean & filtered mean
% figure(5)
% plot(mean(pRVel), mean(pTor), 'b.', mean(tpRVel), mean(tpTor), 'r.')
% 
% 
%     
% %% (0,0) Data 찍어주기.
% [temp_H temp_C] = size(tpRVel);
%  
% StpRVel = [zeros(temp_H, 1), tpRVel(:,:)];
% StpTor = [zeros(temp_H, 1), tpTor(:,:)];
% %% (0,0) 추가된 Graph
% figure(6)
% plot(StpRVel, StpTor, '.')
% 
% xlabel('Measured Link velocity [rad/s]')
% ylabel('Measured Torque [Nm]')
% title('RVel. vs. Tor. with break-away torque')
% 
% %%
% figure(7)
% plot(mean(StpRVel), mean(StpTor), '.')
% 
% xlabel('Shaft input velocity [rad/s]')
% ylabel('mean of Measured Torque [rad/s]')
% title('mean Ref Vel. vs. Tor. with break-away torque')
% 
% 
% %% 마찰 계수 추정
% MVMT_A = [mean(StpRVel); mean(StpTor)]';
% 
% MVMT = [StpRVel(:), StpTor(:)];
% %% Columb + Viscous Friction model
% 
% % % Basic exponential model
% % % x(1) = Fc   coulomb friction
% % % x(2) = Fv   viscous friction
% % 
% % % C-V friction
% % %[x, fval, exitflag] = fminsearch(@(x) cv_fr(x, MVMT_A), [6.6 0.5])
% % 
% % % C-V 2 friction
% % %[x, fval, exitflag] = fminsearch(@(x) cv_fr2(x, MVMT_A), [6.5 0.01 0.001])
% % 
% % % C-V3 friction  (6.63)
% % [x, fval, exitflag] = fminsearch(@(x) cv_fr3(x, MVMT_A), [6.6 8.0 -0.5 0.8])
% % 
% % % validation with experiment data
% % % vel : 0.001 ~ 1.0
% % v = MVMT(:,1);
% % 
% % % Designed Friction model
% % %est_fr = x(1)*sign(v) + x(2)*v;
% % %est_fr = x(1)*sign(v) + x(2)*v + x(3)*(v.^2);
% % est_fr = x(1)*sign(v) + x(2)*v + x(3)*(v.^2) + x(4)*(v.^3);
% % 
% % %
% % % 통합된 Velocity vs. measured Torque graph
% % figure(11)
% % plot(MVMT_A(:,1), MVMT_A(:,2), '.', v, est_fr, 'r.')
% % 
% % 
% % %legend('measured', 'estimated')
% % 
% % xlabel('Shaft mean velocity [rad/s]')
% % ylabel('mean Measured Torque [Nm]')
% % title('estimated static friction')
% % 
% % %%
% % 
% % % Basic exponential model
% % % x(1) = alpha0   coulomb friction
% % % x(2) = alpha1
% % % x(3) = vs
% % % x(4) = alpha2   viscous friction
% % 
% % % 1차 파형
% % %[x, fval, exitflag] = fminsearch(@(x) static_fr(x, MVMT_A), [6.5 1.2 0.2 0.03])
% % [x, fval, exitflag] = fminsearch(@(x) static_fr(x, MVMT_A), [6.0 1.0 0.0064 0.06])
% % 
% % % validation with experiment data
% % % vel : 0.001 ~ 1.0
% % v = MVMT(:,1);
% % 
% % % 1차 파형
% % s_fr = ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v);
% % 
% % %
% % % 통합된 Velocity vs. measured Torque graph
% % figure(11)
% % plot(MVMT_A(:,1), MVMT_A(:,2), '.', v, s_fr, 'r.')
% % 
% % 
% % %legend('measured', 'estimated')
% % 
% % xlabel('Shaft mean velocity [rad/s]')
% % ylabel('mean Measured Torque [Nm]')
% % title('estimated static friction')
% 
% 
% %%
% % Basic exponential model
% % x(1) = alpha0   coulomb friction
% % x(2) = alpha1
% % x(3) = vs
% % x(4) = alpha2   viscous friction
% 
% % 3차 파형 초기값
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.6 0.0 0.001 0.1 0.01 0.01])
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.0 1.0 0.3 0.3 0.1 0.1])
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.0 0.3 0.3 1.3 0.01 0.1])
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.0 1.0 0.03 3.0 0.0 0.0])
% 
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [5.6 0.1 0.019 2.5 0.005 0.1])
% 
% % test3 저속 구간 포함
% [x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.3 10.0 0.02 10.2 3.0 0.5])
% 
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.3 12.9 0.003 10.0 5.0 1.0])
% 
% %[x, fval, exitflag] = fminsearch(@(x) static_fr_mod(x, MVMT_A), [6.3 12.0 0.003 10.0 10.0 0.5])
% 
% 
% 
% 
% 
% % validation with experiment data
% % vel : 0.001 ~ 1.0
% v = MVMT(:,1);
% 
% % calculated friction torque by estimated friction parameters
% % 3차 파형
% s_fr = ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v + x(5)*(v.^2) + x(6)*(v.^3));
% 
% % 2차 파형
% %s_fr = ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v + x(5)*(v.^2));
% 
% % 1차 파형
% %s_fr = ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v);
% 
% %
% % 통합된 Velocity vs. measured Torque graph
% figure(105)
% plot(MVMT_A(:,1), MVMT_A(:,2), '.', v, s_fr, 'r.')
% 
% 
% %legend('measured', 'estimated')
% 
% xlabel('Shaft mean velocity [rad/s]')
% ylabel('mean Measured Torque [Nm]')
% title('estimated static friction')
% 
% % %%
% % figure(106)
% % plot(MVMT(:,1), MVMT(:,2), '.', v, s_fr, 'ro', MVMT_A(:,1), MVMT_A(:,2), 'k.')
% % 
% % legend('measured', 'estimated', 'mean measured')
% % 
% % xlabel('Shaft velocity [rad/s]')
% % ylabel('Measured Torque [Nm]')
% % title('estimated static friction 비교')
% % 
% % %%
% % figure(107)
% % plot(v, s_fr, 'r.')
% % xlabel('Shaft mean velocity [rad/s]')
% % ylabel('mean Measured Torque [Nm]')
% % title('estimated fricttion torque')
% 
% % 최대 속도까지에 대해 마찰 모델식 검증 (behaviour 파악)
% % 4.3633 : shaft 최대 속도 (rad/s)
% v = linspace(0, 4.3633, 1000000);
% 
% vs_fr = ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v + x(5)*(v.^2) + x(6)*(v.^3));
% 
% figure(108)
% plot(v, vs_fr, 'r.')
% 
% xlabel('Ref. Shaft velocity [rad/s]')
% ylabel('Friction Torque [Nm]')
% title('Validation: Ref. Velocity vs. Estimated Friction Torque')