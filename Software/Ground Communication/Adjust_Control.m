function [] = Adjust_Control(varargin)
% Flight Time Simulator
% Balloon modeled as perfect sphere for simplicity
tic
load('GridData.mat')

d0 = datetime('2018-07-09 00:00:00', 'InputFormat', 'yyyy-MM-dd HH:mm:ss') - init_time;

if nargin > 0
    Data = varargin{1};
    alt = Data.alt;
    lat0 = Data.lat;
    long0 = Data.long;
    t0 = Data.time;
else
    alt = 15000; % starting altitude in meters
    lat0 = 33.753746; % starting latitude
    long0 = -84.386330; %starting longitude
    t0 = hours(d0)+1; %initial time in hours
end
x1 = init_lat:fin_lat;
x2 = init_long:fin_long;
x3 = Pressures;
x4 = 0:3:hours(fin_time-init_time);

mask1 = 1:33;
mask2 = 1:33;
mask3 = 1:31;
mask4 = 1:16;

alts = double(interpn(x1, x2, x3, x4, hgt, lat0, long0, Pressures, t0, 'linear'));
alts = reshape(alts, 1, []);

% linear interpolation of pressures to match altitude
Pressure0 = double(interpn(alts, Pressures, alt, 'linear'));
Pressure = Pressure0;
%surface temperature inpertoplation
T0 = double(interpn(x1, x2, x3, x4, tmp, lat0, long0, Pressure0, t0));
RH0 = double(interpn(x1, x2, x3, x4, rh, lat0, long0, Pressure0, t0))/100;

% Initial Conditions

% Payload
Msys = 2.72; % system mass kg
Mbal = 1.2; % balloon mass (known) kg
Mcon = .0927; % connector mass kg

t = t0; % elapsed time in hours initial
dt = 1; % time step is in seconds

Psat = 6.1078*10^((7.5*(T0-273.15))/(T0-273.15+237.3)); % vapor pressure in mb
Pv = RH0*Psat;
Pd = Pressure0-Pv;
pa = ((Pd*100)*MolA+(Pv*100)*MolV)/(R*T0); % density of air
ph = ((Pressure0*100*MolH)/(R*T0)); % density of helium

Vol = 25; % 5.5; % cubic meters
Mhel = ph*Vol; % calculated helium volume
r = ((3*Vol)/(4*pi))^(1/3); % radius in meters
area = pi*r^2;
Fb = g0*Vol*(pa-ph); % buoyancy force
Fg = g0*(Msys+Mcon+Mbal); % gravitational force
Cd = .25; % drag coefficient
Vi = sqrt((Fb-Fg)/(.5*Cd*pa*area));
Fd = .5*Cd*pa*Vi^2*area; % drag force

% Kinematic Values
Mvirt = (Mhel+Msys+Mcon+Mbal) + .37*Vol*pa; % virtual mass includes displaaced air
A = (Fb-Fg-Fd)/Mvirt; % acceleration
Vf = 0; % final velocity
Dz = 0; % displacement in z
Dx = 0; % displacement in x
Dy = 0; % displacement in y
% counters
count = 0;

% states
up = false;
burst = false;
% save data
data_plot = zeros(40000, 5);
t_burst = 0;

% control
ballast_score = 0;

helium_score = 0;

Bal = 1; % starting mass of ballast in kg
minBal = .8e-3; % ballast drop mass in kg
minHel = .001; % mass of vented helium in kg

minAlt = 20000;
maxAlt = 22000;
deltaAlt = 0;
minVel = 1;
Vel = 0;
minAcc = 0;
Acc = 0;
% ballast
BallastAlpha = .003; % altitude
BallastBeta = 2.5; % velocity 
BallastGamma = 0; % acceleration
% helium
HeliumAlpha = .01; % altitude
HeliumBeta = 3; % velocity
HeliumGamma = 0; % acceleration

n = size(rh, 3);
m = size(rh, 4);

hgt = hgt(3, 3, :, :);
hgt = reshape(hgt, n, m);
hgt = hgt(:, 1:5);

tmp = tmp(3, 3, :, :);
tmp = reshape(tmp, n, m);
tmp = tmp(:, 1:5);

rh = rh(3, 3, :, :);
rh = reshape(rh, n, m);
rh = rh(:, 1:5);

time = t0;

controlTime = 30;

figure('units','normalized','outerposition',[0 0 1 1])
subplot(2,3,[1,2,4,5])
altPlot = plot(time,alt); % plot in those axes
title('Altitude')
subplot(2,3,3)
velPlot = plot(time,Vi); % plot in those axes
title('Velocity')
subplot(2,3,6)
accPlot = plot(time,A); % plot in those axes
title('Acceleration')
while Bal > 0 && count < 40000 && Pressure < 950
 
    count = count + 1;
    
    % Saturate Acceleration and Velocity
    if A > 10
        A = 10;
    elseif A < -10
        A = -10;
    end
    
    if Vi > 20
        Vi = 20;
    elseif Vi < -20
        Vi = -20;
    end
    
    t = t + dt/3600; % set new time
    Dz = Vi*dt + .5*A*dt^2; % displacement over last interval
    alt = alt + Dz;
    
    % itterative check for pressure matching the new altitude 
    % interpolate data grid to get table of altitudes at pressure levels just below previous pressure
    
    temp3 = mask3(x3>=Pressure);
    prs = (temp3(end)-1):(temp3(end)+1);
    
    temp4 = mask4(x4>=t);
    s_t = (temp4(1)-1):(temp4(1)+1);

    alts = reshape((double(interpn(x3(prs), x4(s_t), hgt(prs, s_t), x3(prs), t0+t, 'linear'))*dt),1,[]);
    
    
    % linear interpolation of pressures to match altitude exactly
    Pressure = double(interpn(alts, x3(prs), alt, 'linear', Pressure0));    


    T = double(interpn(x3(prs), x4(s_t), tmp(prs, s_t), Pressure, t0+t, 'linear', T0)); % Temperature
    
    if T < 200
        error('Temp out of range')
    end
    
    RH = double(interpn(x3(prs), x4(s_t), rh(prs, s_t), Pressure, t0+t, 'linear', RH0))/100; % Relative humidity
    
    Psat = 6.1078*10^((7.5*(T-273.15))/(T-273.15+237.3)); % saturation vapor pressure in mb
    Pv = RH*Psat; % partial pressure of vapor
    Pd = Pressure-Pv; % partial pressure of dry air
    pa = ((Pd*100)*MolA+(Pv*100)*MolV)/(R*T); % density of air
    ph = ((Pressure*101)*MolH)/(R*T); % density of helium
    
    if ~burst
        Vol = .99*(Mhel/ph); % volume of balloon
        r = ((3*Vol)/(4*pi))^(1/3); % radius in meters
        Mvirt = (Mhel+Msys+Mcon+Mbal) + .37*Vol*pa; % virtual mass includes displaced air
        Fb = g0*Vol*(pa-ph); % buoyancy force
        Fg = g0*(Msys+Mcon+Mbal); % gravitational force
        Fd = .5*pa*Vi^2*Cd*(r^2*pi); % drag force
    else
        Vol = 0; % volume of balloon
        r = 0; % radius in meters
        Mvirt = (Msys+Mcon+Mbal); % virtual mass includes displaced air
        Fg = g0*(Msys+Mcon+Mbal); % gravitational force
        Fd = .5*pa*Vi^2*Cd*(.75^2*pi); % drag force
        Fb = 0;
    end

    if r > (8.63/2) % based on 11 foot burst diameter
        burst = true;
        t_burst = t;
        Mhel = 0;
    end
    
    if alt > minAlt && ~up
        up = true;
    end
    
    if up && mod(count, round(controlTime/dt)) == 0 && ~burst
        if alt < minAlt 
            deltaAlt = alt - minAlt;
        elseif alt > maxAlt
            deltaAlt = alt - maxAlt;
        else
            deltaAlt = 0;
        end
        
%         midAlt = (maxAlt - minAlt)+minAlt;
%         midDif = abs(alt- midAlt)/(maxAlt-minAlt)/2;
%         minVel2 = minVel*midDif;
        
        if Vf > minVel || Vf < -minVel
            Vel = Vf;
        else
            Vel = 0;
        end
        
        if A > minAcc || A < -minAcc
            Acc = A;
        else
            Acc = 0;
        end
            
        ballast_score = BallastAlpha*deltaAlt +  ...
                        BallastBeta*Vel + ...
                        BallastGamma*Acc;
                    
        helium_score = HeliumAlpha*deltaAlt +  ...
                       HeliumBeta*Vel + ...
                       HeliumGamma*Acc;
                   
        if helium_score > controlTime
            helium_score = controlTime;
        end
        
        if ballast_score < -2*controlTime
            ballast_score = -2*controlTime;
        end
                   
        if ballast_score <= 0 
            Mbal =  Mbal - minBal*round(-ballast_score)
        end
        
        if helium_score >= 0
            Mhel =  Mhel - helium_score*minHel;
        end
          
    end
    
    data_plot(count, :) = [alt, Vi, A, ballast_score, helium_score];
    
    if mod(count, 100) == 0
        set(altPlot,'XData',time,'YData',data_plot(1:count, 1)) % change the line data
        set(velPlot,'XData',time,'YData',data_plot(1:count, 2)) % change the line data
        set(accPlot,'XData',time,'YData',data_plot(1:count, 3)) % change the line data
        drawnow
    end
    
    time = [time, t];
    
    if Vi > 0
        A = (Fb-Fg-Fd)/Mvirt; % acceleration
    else
        A = (Fb-Fg+Fd)/Mvirt;
    end
    Vf = Vi + A*dt; % new final velocity
    Vi= Vf;
end

fprintf('Flight time is %f minutes', (t-t0)*60) 

time = time(1:end-1);

set(altPlot,'XData',time,'YData',data_plot(1:count, 1)) % change the line data
set(velPlot,'XData',time,'YData',data_plot(1:count, 2)) % change the line data
set(accPlot,'XData',time,'YData',data_plot(1:count, 3)) % change the line data
drawnow
        
figure('Name','Ballast Score')
plot(data_plot(1:count,4))
figure('Name','Helium Score')
plot(data_plot(1:count,5))

toc
end




