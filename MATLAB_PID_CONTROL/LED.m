clear all
clc
syms s
%G = 1/ (s^2+3S+1)
num=1;
den = sym2poly(s^2+3*s+1);
G=tf(num,den);
H=10;
Kp=1;
Ki=0;
Kd=0;
C=pid(Kp,Ki,Kd);
T= feedback(C*G,H);
step(T);