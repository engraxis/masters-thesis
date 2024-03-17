close all;clear all;clc;

nUC = load('nMVMF_UC.mat');
figure(1);
plot(nUC.tmVF(:,1),nUC.tmVF(:,2),'b.')
hold on;


nC = load('nMVMF_C.mat');
plot(nC.tmVF(:,1),nC.tmVF(:,2),'r.')

line([0 -160],[-0.1 -0.1],'color','b')
line([0 -160],[0.1 0.1],'color','b')

xlabel('Reference Velocity [mm/s]')
ylabel('Torque [N]')
title('Negative Velocities - PULL')

legend('Uncompensated', 'Compensated')
hold off



pUC = load('pMVMF_UC.mat');
figure(2);
plot(pUC.tmVF(:,1),pUC.tmVF(:,2),'b.')
hold on;


pC = load('pMVMF_C.mat');
plot(pC.tmVF(:,1),pC.tmVF(:,2),'r.')

line([0 160],[-0.1 -0.1],'color','b')
line([0 160],[0.1 0.1],'color','b')

xlabel('Reference Velocity [mm/s]')
ylabel('Torque [N]')
title('Positive Velocities - PUSH')


legend('Uncompensated', 'Compensated')
hold off