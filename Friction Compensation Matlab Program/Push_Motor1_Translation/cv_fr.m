function f = cv_fr(x,a)
% Coulomb & Viscous friction
% Ff = Fc*sgn(v) + Fv*v
% x(1) = Fc
% x(2) = Fv

% v(1) = velocity input
% v(2) = torque input

% f is the objective function

% v input 은 2 개의 vector 이므로,
tr = a(:,2);
v = a(:,1);

%f = (tr - ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v))' * (tr - ((x(1) + x(2) * exp(-(v/x(3)).^2)).*sign(v) + x(4)*v));

f = (tr - ((x(1)*sign(v) + x(2)*v)))' * (tr - ((x(1)*sign(v) + x(2)*v)));