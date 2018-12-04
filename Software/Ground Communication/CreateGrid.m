function [ ] = CreateGrid( )
% Flight Time Simulator

% Balloon modeled as perfect sphere for simplicity
% Parameters
Pressures = [250,500,700,850];

% Constants
g0 = 9.80665; % standard gravity in m/s^2
R = 8.314; % Gas Constnat in J/mol
MolH = 0.004003; % Molar mass Helium kg/mol
MolA = 0.0289647; % Molar weight Air kg/mol
MolV = 0.018016; % Molar mass VApor kg/mol

% Input Parameters
Base = 'datafiles/ncar_3km_2017091400_mem10_f0';
Tstart = 05; % data starting time in hours
Tend = 10; % data ending time in hours
init_lat = 17; % starting latitude
fin_lat = 49;
init_long = -100; %starting longitude
fin_long = -68;
% date time value of starting time following grib2 format
init_time = datetime('2017-09-14 05:00:00', 'InputFormat', 'yyyy-MM-dd HH:mm:ss');
fin_time = datetime('2017-09-14 10:00:00', 'InputFormat', 'yyyy-MM-dd HH:mm:ss'); 
tend = (hours(fin_time - init_time)+1);

tmp = zeros((fin_lat-init_lat), (fin_long-init_long), 31, tend);
rh = zeros((fin_lat-init_lat), (fin_long-init_long), 31, tend);
ugrd = zeros((fin_lat-init_lat), (fin_long-init_long), 31, tend);
vgrd = zeros((fin_lat-init_lat), (fin_long-init_long), 31, tend);
hgt = zeros((fin_lat-init_lat), (fin_long-init_long), 31, tend);

% Import Data from CSV Files and
% Construct Arrays for Each Data Field
for set = Tstart:3:Tend
    file = [Base, num2str(set, '%03.0f'), '.txt'];
    FID = fopen(file, 'r+');
    C = textscan(FID, '%*q %q %q %q %d8 %d8 %f32', 'Delimiter', ',');
    fclose(FID);
    % C row -> [{time}, {code}, {mb level}, {lat}, {long}, {value}]
    [row, ~] = size(C{1,1});
    for r = 1:row
        % find the time, latitude, longitude, and pressure
        dd = datetime(C{1,1}{r}, 'InputFormat', 'yyyy-MM-dd HH:mm:ss') - init_time;
        dh = hours(dd)/3 + 1;
        dlong = C{1,4}(r) - init_long + 1;
        dlat = C{1,5}(r) - init_lat + 1;
        pres = strrep(C{1,3}{r}, ' mb', '');
        mb = 0;
        switch pres
            case '1000'
                mb = 1;
            case '975'
                mb = 2;
            case '950'
                mb = 3;
            case '925'
                mb = 4;
            case '900'
                mb = 5;
            case '850'
                mb = 6;
            case '800'
                mb = 7;
            case '750'
                mb = 8;
            case '700'
                mb = 9;
            case '650'
                mb = 10;
            case '600'
                mb = 11;
            case '550'
                mb = 12;
            case '500'
                mb = 13;
            case '450'
                mb = 14;
            case '400'
                mb = 15;
            case '350'
                mb = 16;
            case '300'
                mb = 17;
            case '250'
                mb = 18;
            case '200'
                mb = 19;
            case '150'
                mb = 20;
            case '100'
                mb = 21;
            case '70'
                mb = 22;
            case '50'
                mb = 23;
            case '30'
                mb = 24;
            case '20'
                mb = 25;    
            case '10'
                mb = 26;
            case '7'
                mb = 27;
            case '5'
                mb = 28;
            case '3'
                mb = 29;
            case '2'
                mb = 30;    
            case '1'
                mb = 31;
        end
        % check code for data type and input value into array
        % array is code[lat, long, pressure, time]
        switch C{1,2}{r} 
            case 'TMP'
                tmp(dlat, dlong, mb, dh) = C{1,6}(r); 
            case 'RH'
                rh(dlat, dlong, mb, dh) = C{1,6}(r); 
            case 'UGRD'
                ugrd(dlat, dlong, mb, dh) = C{1,6}(r);  
            case 'VGRD'
                vgrd(dlat, dlong, mb, dh) = C{1,6}(r); 
            case 'HGT'
                hgt(dlat, dlong, mb, dh) = C{1,6}(r); 
        end
    end
end

% HGT is as accurate as the barometric formula, so it is used instead
% [lats, longs, pres, hrs] = size(hgt);
% 
% altd = zeros(lats,longs,pres,hrs);
% altd2 = zeros(lats,longs,pres,hrs);
% 
% for a = 1:lats
%     for b = 1:longs
%         for c = 1:pres
%             for d = 1:hrs
%                 T1 = tmp(a,b,c,d); % temperature
%                 P1 = Pressures(c);
%                 % Barometric Pressure Formula
%                 if c < 19
%                     altd(a,b,c,d) = (T1/(-.0065))*((P1/1013.25)^((-R*(-.0065))/(g0*MolA))-1);
%                 elseif c < 23
%                     altd(a,b,c,d) = 11e3 + ((R*T1)/(-g0*MolA))*log(P1/226.321);
%                 elseif c < 27
%                     altd(a,b,c,d) = 20e3 +(T1/.001)*((P1/54.7489)^((-R*.001)/(g0*MolA))-1);
%                 else
%                     altd(a,b,c,d) = 32e3 + (T1/.0028)*((P1/8.6802)^((-R*.0028)/(g0*MolA))-1);
%                 end
%                 
%                 if c < 19
%                     altd2(a,b,c,d) = (288.15/(-.0065))*((P1/1013.25)^((-R*(-.0065))/(g0*MolA))-1);
%                 elseif c < 23
%                     altd2(a,b,c,d) = 11e3 + ((R*216.65)/(-g0*MolA))*log(P1/226.321);
%                 elseif c < 27
%                     altd2(a,b,c,d) = 20e3 +(216.65/.001)*((P1/54.7489)^((-R*.001)/(g0*MolA))-1);
%                 else
%                     altd2(a,b,c,d) = 32e3 + (228.65/.0028)*((P1/8.6802)^((-R*.0028)/(g0*MolA))-1);
%                 end
%             end
%         end
%     end
% end

clear('C', 'base', 'file', 'FID', 'mb', 'pres', 'dd', 'dh', 'dlong', 'dlat', 'r', 'set', 'Tstart', 'Tend', 'lats', 'longs', 'pres', 'hrs');
save('GridData.mat');

end

