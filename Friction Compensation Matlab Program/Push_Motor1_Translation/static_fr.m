function f = static_LuGre(x,a)
% x(1) = alpha0
% x(2) = alpha1
% x(3) = vs
% x(4) = alpha2

% V = velocity input
% F = measured friction

% f is the objective function
%f = (F - ((x(1) + x(2)).*exp(-(V/x(3)).^2)*sign(V) + x(4).*V)).^2;

% v input 은 2 개의 vector 이므로,
F = a(:,2);
V = a(:,1);

% cost function
f = (F - ((x(1) + x(2)*exp(-(V/x(3)).^2)).*sign(V) + x(4)*V))' * (F - ((x(1) + x(2)*exp(-(V/x(3)).^2)).*sign(V) + x(4)*V));



%f = (((v(1) - ((x(1) + x(2) * exp(-(v(2)/x(3)).^2)).*sign(v(2)) + x(4)*v(2)))).^2)' * ((v(1) - ((x(1) + x(2) * exp(-(v(2)/x(3)).^2)).*sign(v(2)) + x(4)*v(2))).^2);


%f = (v(2) - ((x(1) + x(2) * exp(-(v(1)/x(3)).^2)).*sign(v(1)) + x(4)*v(1)))' * (v(2) - ((x(1) + x(2) * exp(-(v(1)/x(3)).^2)).*sign(v(1)) + x(4)*v(1)));