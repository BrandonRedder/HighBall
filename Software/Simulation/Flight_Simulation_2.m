function [] = Flight_Simulation_2()
% Flight Time Simulator
% Balloon modeled as perfect sphere for simplicity

tic

load('GridData.mat')
Pressures = [1000:-25:900, 850:-50:100, 70, 50, 30, 20, 10, 7, 5, 3, 2, 1];

alt = 1000; % starting altitude in meters
lat0 = 33.753746; % starting latitude
long0 = -84.386330; %starting longitude
d0 = datetime('2018-07-09 00:00:00', 'InputFormat', 'yyyy-MM-dd HH:mm:ss') - init_time;
t0 = hours(d0); %initial time in hours

x1 = init_lat:fin_lat;
x2 = init_long:fin_long;
x3 = Pressures;
x4 = 0:3:hours(fin_time-init_time);

mask1 = 1:33;
mask2 = 1:33;
mask3 = 1:31;
mask4 = 1:16;

alts = double(interpn(x1, x2, x3, x4, hgt, lat0, long0, Pressures, t0, 'makima'));
alts = reshape(alts, 1, []);

% plot(Pressures,alts,'*');
% xlabel('pressure')
% ylabel('altitude')

% linear interpolation of pressures to match altitude
P0 = double(interpn(alts, Pressures, alt, 'makima'));
%surface temperature inpertoplation
T0 = double(interpn(x1, x2, x3, x4, tmp, lat0, long0, P0, t0));
RH0 = double(interpn(x1, x2, x3, x4, rh, lat0, long0, P0, t0))/100;

% Initial Conditions

% Payload
Msys = 2.72; % system mass kg
Mbal = 1.2; % balloon mass (known) kg
Mcon = .0927; % connector mass kg
Bal = 1; % starting mass of ballast in kg
MinBal = .6e-3; % ballast drop mass in kg
MinHel = .01; % mass of vented helium in kg

t = 5; % elapsed time in hours initial
dt = .5; % time step is in seconds

Psat = 6.1078*10^((7.5*(T0-273.15))/(T0-273.15+237.3)); % vapor pressure in mb
Pv = RH0*Psat;
Pd = P0-Pv;
pa = ((Pd*100)*MolA+(Pv*100)*MolV)/(R*T0); % density of air
ph = ((P0*100*MolH)/(R*T0)); % density of helium

Vol = 5.5; % cubic meters
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
hcount = 0;
bcount = 0;
% states
up = false;
burst = false;
% save data
data_plot = zeros(15,2000);
t_burst = 0;
while Bal > 0 && count < 2000 && P0 < 950
 
    count = count + 1;

    hcount = hcount + 1;
    bcount = bcount + 1;
    
    t = t + dt/3600; % set new time
    Dz = Vi*dt + .5*A*dt^2; % displacement over last interval
    alt = alt + Dz;
    % Convert from meters to latitude and longitude degrees
    % doesn't account for altitude, but less than 0.5% error
    % https://en.wikipedia.org/wiki/Geographic_coordinate_system#Expressing_latitude_and_longitude_as_linear_units
    dlat = (111132.92-559.82*cosd(2*lat0)+1.175*cosd(4*lat0)-0.0023*cosd(6*lat0));
    lat0 = lat0 + Dx/dlat;
    dlong = (111412.84*cosd(lat0)-93.5*cosd(3*lat0)+0.118*cosd(5*lat0));
    long0 = long0 + Dy/dlong;
    
    % itterative check for pressure matching the new altitude 
    % interpolate data grid to get table of altitudes at pressure levels just below previous pressure
    
%     prs = (1.2*P0):(-P0/100):(P0*.8);
%     alts = double(interpn(x1, x2, x3, x4, hgt, lat0, long0, prs, t+t0));
%     alts = reshape(alts, 1, []);
%     alts = alts(alts>0);
%     prs = prs(alts>0);

    temp1 = mask1(x1>=lat0);
    s_lat = (temp1(1)-2):(temp1(1)+2);
    
    temp2 = mask2(x2>=long0);
    s_long = (temp2(1)-2):(temp2(1)+2);
    
    temp3 = mask3(x3>=P0);
    prs = (temp3(end)-2):(temp3(end)+2);
    
    temp4 = mask4(x4>=t);
    s_t = (temp4(1)-2):(temp4(1)+2);

    alts = reshape((double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), hgt(s_lat, s_long, prs, s_t), lat0, long0, x3(prs), t0+t, 'makima'))*dt),1,[]);
    
    % linear interpolation of pressures to match altitude exactly
    P0 = double(interpn(alts, x3(prs), alt, 'makima'));    

    Dx = double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), ugrd(s_lat, s_long, prs, s_t), lat0, long0, P0, t0+t, 'makima'))*dt;
    Dy = double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), vgrd(s_lat, s_long, prs, s_t), lat0, long0, P0, t0+t, 'makima'))*dt;
    
    T = double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), tmp(s_lat, s_long, prs, s_t), lat0, long0, P0, t0+t, 'makima')); % Temperature
    
    if T < 200
        error('Temp out of range')
    end
    
    RH = double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), rh(s_lat, s_long, prs, s_t), lat0, long0, P0, t0+t, 'makima'))/100; % Relative humidity
    
    Psat = 6.1078*10^((7.5*(T-273.15))/(T-273.15+237.3)); % saturation vapor pressure in mb
    Pv = RH*Psat; % partial pressure of vapor
    Pd = P0-Pv; % partial pressure of dry air
    pa = ((Pd*100)*MolA+(Pv*100)*MolV)/(R*T); % density of air
    ph = ((P0*101)*MolH)/(R*T); % density of helium
    
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
    
    data_plot(:,count) = [alt, P0, Dx, Dy, Dz, Vf, A, Fb, Fg, Fd, lat0, long0, Vol, r, T];
    
    if Vi > 0
        A = (Fb-Fg-Fd)/Mvirt; % acceleration
    else
        A = (Fb-Fg+Fd)/Mvirt;
    end
    Vf = Vi + A*dt; % new final velocity
    Vi= Vf;

    if r > (8.63/2) % based on 11 foot burst diameter
        burst = true;
        t_burst = t;
    end
%     if alt > 18000 && ~up
%         up = true;
%         Mhel = Mhel - .05;
%     end
%     
%     if alt > 23000 && Vi > 0 && hcount > 15
%         Mhel = Mhel - MinHel*ph;
%         hcount = 0;
%     end
%     
%     if alt < 17000 && up && Vi < 0 && bcount > 15 
%         Bal = Bal - MinBal;
%         Msys = Msys - MinBal;
%         bcount = 0;
%     end
end
% 3D scatter plot of relative position during flight
h = figure;
ax = axes;
scatter3(ax, data_plot(11,1:count-1), data_plot(12,1:count-1), data_plot(1,1:count-1),10,'filled')
xlabel('Latitude');
ylabel('Longitude');
zlabel('Altitude');
axis tight manual % this ensures that getframe() returns a consistent size
filename = 'testAnimated.gif';
x = 33.59;
y = -84.45;
z = 24631.57;
for var = 0:(pi/50):pi
    % Capture the plot as an image
    [theta,rho,p] = cart2pol(x,y,z);
    [x,y,z] = pol2cart(theta+var,rho,p);
    campos(ax, [x,y,z])
    drawnow
    frame = getframe(h);
    im = frame2im(frame);
    [imind,cm] = rgb2ind(im,256);
    % Write to the GIF File
    if theta == 0
        imwrite(imind,cm,filename,'gif', 'Loopcount',inf);
    else
        imwrite(imind,cm,filename,'gif','WriteMode','append');
    end
end

% figure('Name','Altitude')
% plot(data_plot(1,1:count))
% figure('Name','Pressure')
% plot(data_plot(2,1:count))
% figure('Name','Velocity')
% plot(data_plot(6,1:count))
% figure('Name','Acceleration')
% plot(data_plot(7,1:count))
% figure('Name','Temperature')
% plot(data_plot(15,1:count))
% figure('Name','Change Latitude')
% plot(data_plot(11,1:count))
% figure('Name','Change Longitude')
% plot(data_plot(12,1:count))

toc
end




