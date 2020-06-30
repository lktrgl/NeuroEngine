clear all

##----------------- Global declarations ----------------------------------------

global t0=0;
global t1=4;
NumP=100;
global A=1;
global tImp=(t1-t0)/2;

global T=0.09;
global mju=1.2;
global k=1;

##---------------- Distortion --------------------------------------------------

function y=xin(t)
  global t0;
  global A;
  global tImp;

  y=A*(sign(t-t0)-sign((t-t0)-tImp));

endfunction

##----------------- Right Part -------------------------------------------------

function xdot = SYSTEM (y, t)
  global mju;
  global T;
  global k;

  xdot = zeros (2,1);
  xdot(1) = y(2);
  xdot(2) = -mju./T.*y(2) -1./(T.*T).*y(1) + k./(T.*T).*xin(t);

endfunction 

##---------------- Solving -----------------------------------------------------


t = linspace (t0, t1, NumP);
sol = lsode ("SYSTEM", [0,0], t); 
sol_coord=sol(:,1);
xinput = xin(t);
sol_mistake=sol_coord - xinput';

##---------------- Figure 1 ----------------------------------------------------

figure(1);
plot( t, sol_coord,"r",t, xinput,"b")
title ("Transition process h(t) vs input(t)");
ylabel ("h(t)");
xlabel ("t");

##---------------- Figure 2 ----------------------------------------------------

figure(2);
plot( t, sol_mistake,"m",t, xinput,"b")
title ("Transition error vs input(t)");
ylabel ("h(t) - input(t)");
xlabel ("t");

