s = tf('s');
To = 1;                 % ambient/initial temperature
K = 500;               % DC gain
tau = 0.1;               % time constant
P = K/(tau*s+1);        % model transfer function
[y,t] = step(P,350);    % model step response
plot(t+50,y+To);
hold
plot(temp,'r:')
xlabel('time (sec)')
ylabel('temperature (degrees C)')
title('Lightbulb Temperature Step Response')
legend('model','experiment','Location','SouthEast')