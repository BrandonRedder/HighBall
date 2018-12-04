function [] = Plot_Weather_Data()
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

% Dx = double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), ugrd(s_lat, s_long, prs, s_t), lat0, long0, P0, t0+t, 'makima'))*dt;
% Dy = double(interpn(x1(s_lat), x2(s_long), x3(prs), x4(s_t), vgrd(s_lat, s_long, prs, s_t), lat0, long0, P0, t0+t, 'makima'))*dt;
a=10;b=25;c=10;d=25;e=25;f=3;g=.5;

u = interpn(x1(a:b), x2(c:d), x3(e-1:e+1), x4(f-1:f+1), ugrd(a:b,c:d,(e-1:e+1),(f-1:f+1)), (x1(a)+1):g:(x1(b)-1), (x2(c)+1):g:(x2(d)-1), x3(e), x4(f));
v = interpn(x1(a:b), x2(c:d), x3(e-1:e+1), x4(f-1:f+1), vgrd(a:b,c:d,(e-1:e+1),(f-1:f+1)), (x1(a)+1):g:(x1(b)-1), (x2(c)+1):g:(x2(d)-1), x3(e), x4(f));

u2 = interpn(x1(a:b), x2(c:d), x3(e-1:e+1), x4(f-1:f+1), ugrd(a:b,c:d,(e-1:e+1),(f-1:f+1)), (x1(a)+1):g:(x1(b)-1), (x2(c)+1):g:(x2(d)-1), x3(e), x4(f), 'spline');
v2 = interpn(x1(a:b), x2(c:d), x3(e-1:e+1), x4(f-1:f+1), vgrd(a:b,c:d,(e-1:e+1),(f-1:f+1)), (x1(a)+1):g:(x1(b)-1), (x2(c)+1):g:(x2(d)-1), x3(e), x4(f), 'spline');

hold on
quiver((x1(a)+1):g:(x1(b)-1), (x2(c)+1):g:(x2(d)-1),u,v)
quiver((x1(a)+1):g:(x1(b)-1), (x2(c)+1):g:(x2(d)-1),u2,v2)
legend('Linear', 'Spline')
end
