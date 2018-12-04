% parse data

mails = ReadOutlook(...
    'Folder', 'Senior Design SBD');

[r, ~] = size(mails);

Data = cell(r,1);

Time = cell(r,1);

i = 1;

for v = 1:r
    line = mails{v,2};
    
    time = strfind(line, 'Transmit Time: ');
    data = strfind(line, 'Data: ');
    fail = strfind(line, 'No Data');
    newline = strfind(line, [13, 10]);
    
    if isempty(fail)
        t = line(time(1)+15:newline(4)-6);
        Time{i} = [t(1:10), ' ', t(12:end)];
        Data{i} = line(data(1)+6:data(1)+66);
        i = i + 1;
    end
end
i = i-1;

if r > i
    Data = Data(1:i);
    Time = Time(1:i);
end

Parsed_Data = zeros(i,9);
RealTimes = zeros(i,1);

for v = 1:i
    hex = Data{v};
    split = hex(1:2);
    for f = 3:2:59
        split = [split; hex(f:f+1)];
    end
    bin = hexToBinaryVector(split);
    bin2 = [];
    for f = 1:30
        bin2 = [bin2, bin(f,1:8)];
    end
    
    Parsed_Data(v,:) = decode_message(bin2);
    RealTimes(v) = getTime(Time{v});
    
end

time = datenum('2018-11-17 13:00:00','yyyy-mm-dd HH:MM:SS');

RealTimes = (RealTimes- time);

[RealTimes, I] = sort(RealTimes);
Parsed_Data = Parsed_Data(I,:);

mask = (RealTimes > 0);

FinalTimes = RealTimes(mask);
FinalTimes = FinalTimes(end:-1:1);
FinalData = Parsed_Data(mask, :);
[r, c] = size(FinalData);
FinalData = FinalData(r:-1:1, :);

% figure('Name','Temperature','NumberTitle','off');
% plot(FinalTimes, FinalData(:,1))
% title('Temperature')
% figure('Name','Pressure','NumberTitle','off');
% plot(FinalTimes, FinalData(:,2))
% title('Pressure')
% figure('Name','Altitude','NumberTitle','off');
% plot(FinalTimes, FinalData(:,3))
% title('Altitude')
% figure('Name','Latitude','NumberTitle','off');
% plot(FinalTimes, FinalData(:,4))
% title('Latitude')
% figure('Name','Longitude','NumberTitle','off');
% plot(FinalTimes, FinalData(:,5))
% title('Longitude')
% figure('Name','Velocity','NumberTitle','off');
% plot(FinalTimes, FinalData(:,6))
% title('Velocity')
% figure('Name','Helium','NumberTitle','off');
% plot(FinalTimes, FinalData(:,7))
% title('Helium')
% figure('Name','Ballast','NumberTitle','off');
% plot(FinalTimes, FinalData(:,8))
% title('Ballast')
% figure('Name','Control','NumberTitle','off');
% plot(FinalTimes, FinalData(:,9))
% title('Control')



function time = getTime(string)
time = datenum(string,'yyyy-mm-dd HH:MM:SS');
end

function parsed = decode_message(Data)

Encoded = Data(1:end);

parsed = zeros(1,9);

% Temperature Data
TEMP_MIN = -88;
TEMP_STEP = .25;
TEMP_OFFSET = 0;
TEMP_BITS = 9;
parsed(1) = decode(TEMP_MIN, TEMP_STEP, Encoded(TEMP_OFFSET+1:TEMP_OFFSET+TEMP_BITS));

% Pressure Data
PRS_MIN = 0;
PRS_STEP = .25;
PRS_OFFSET = 9;
PRS_BITS = 12;
parsed(2) = decode(PRS_MIN, PRS_STEP, Encoded(PRS_OFFSET+1:PRS_OFFSET+PRS_BITS));

% GPS Data
ALT_MIN = 0;
ALT_STEP = 9;
ALT_OFFSET = 59;
ALT_BITS = 14;
parsed(3) = decode(ALT_MIN, ALT_STEP, Encoded(ALT_OFFSET+1:ALT_OFFSET+ALT_BITS));

LAT_MIN = 18;
LAT_STEP = .000005;
LAT_OFFSET = 73;
LAT_BITS = 23;
parsed(4) = decode(LAT_MIN, LAT_STEP, Encoded(LAT_OFFSET+1:LAT_OFFSET+LAT_BITS));

LONG_MIN = -100;
LONG_STEP = .000005;
LONG_OFFSET = 96;
LONG_BITS = 23;
parsed(5) = decode(LONG_MIN, LONG_STEP, Encoded(LONG_OFFSET+1:LONG_OFFSET+LONG_BITS));

% Velovity Data
VEL_MIN = 0;
VEL_STEP = 0.0005;
VEL_OFFSET = 119;
VEL_BITS = 16;
parsed(6) = decode(VEL_MIN, VEL_STEP, Encoded(VEL_OFFSET+1:VEL_OFFSET+VEL_BITS));

% Helium Data
HEL_MIN = 0;
HEL_STEP = .1;
HEL_OFFSET = 135;
HEL_BITS = 10;
parsed(7) = decode(HEL_MIN, HEL_STEP, Encoded(HEL_OFFSET+1:HEL_OFFSET+HEL_BITS));

% Ballast Data
BALL_MIN = 0;
BALL_STEP = .1;
BALL_OFFSET = 145;
BALL_BITS = 10;
parsed(8) = decode(BALL_MIN, BALL_STEP, Encoded(BALL_OFFSET+1:BALL_OFFSET+BALL_BITS));

% Control Mode Data
CTRL_OFFSET = 175;
CTRL_BITS = 2;
parsed(9) = decode(0, 1, Encoded(CTRL_OFFSET+1:CTRL_OFFSET+CTRL_BITS));

end

function value = decode(min, step, bin)
input = log2dec(bin);
value = (input*step)+min;
end

function [dec] = log2dec(bin)
long = length(bin);
swap = long:-1:1;
dec = 0;
for i = 1:long
    if bin(swap(i))
        dec = dec + 2^(i-1);
    end
end
end

function [bin] = int2bin(integer, long)
integer = round(integer);
if integer == 0
    bin = zeros(1,long);
else
    l = floor(log2(integer));
    
    integer = integer - 2^l;
    bin = true(1,l+1);
    for val = l-1:-1:0
        if (integer < 2^val)
            bin(l+1-val) = false;
        else
            integer = integer - 2^val;
        end
    end
    if length(bin) ~= long
        start = false(1,(long-l-1));
        bin = [start, bin];
    end
end
end

