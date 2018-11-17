hex = '3E81900C3500186A00190320780A050280F1904050000000000000000000';
% hex = 'AAAAAAAAAAAA0000000000000000000000000000000000000000000000000';
split = hex(1:2);
for v = 3:2:59
    split = [split; hex(v:v+1)];
end
bin = hexToBinaryVector(split);
bin2 = [];
for v = 1:30
    bin2 = [bin2, bin(v,1:8)];
end

decode_message(bin2)

function [] = decode_message(Data)

Encoded = Data(1:end);

% Altitude
ALT_MIN = 0;
ALT_STEP = 5;
ALT_OFFSET = 0;
ALT_LENGTH = 13;
ALT = decode(ALT_MIN, ALT_STEP, Encoded(ALT_OFFSET+1:ALT_OFFSET+ALT_LENGTH))

% altitude_buffer
ALT_BUF_MIN = 0;
ALT_BUF_STEP = 5;
ALT_BUF_OFFSET = 13;
ALT_BUF_LENGTH = 11;
ALT_BUF = decode(ALT_BUF_MIN, ALT_BUF_STEP, Encoded(ALT_BUF_OFFSET+1:ALT_BUF_OFFSET+ALT_BUF_LENGTH))

% lat_degree
LAT_MIN = 18;
LAT_STEP = .000005;
LAT_OFFSET = 24;
LAT_LENGTH = 23;
LAT = decode(LAT_MIN, LAT_STEP, Encoded(LAT_OFFSET+1:LAT_OFFSET+LAT_LENGTH))

% long_degree
LONG_MIN = -100;
LONG_STEP = .000005;
LONG_OFFSET = 47;
LONG_LENGTH = 23;
LONG = decode(LONG_MIN, LONG_STEP, Encoded(LONG_OFFSET+1:LONG_OFFSET+LONG_LENGTH))

% cutdown
CUT_MIN = 0;
CUT_STEP = 1;
CUT_OFFSET = 70;
CUT_LENGTH = 2;
CUT = decode(CUT_MIN, CUT_STEP, Encoded(CUT_OFFSET+1:CUT_OFFSET+CUT_LENGTH))

% update_rate
UPD_MIN = 0;
UPD_STEP = 1;
UPD_OFFSET = 72;
UPD_LENGTH = 10;
UPD = decode(UPD_MIN, UPD_STEP, Encoded(UPD_OFFSET+1:UPD_OFFSET+UPD_LENGTH))

% hel_alpha
HEL_A_MIN = 0;
HEL_A_STEP = .01;
HEL_A_OFFSET = 82;
HEL_A_LENGTH = 10;
HEL_A = decode(HEL_A_MIN, HEL_A_STEP, Encoded(HEL_A_OFFSET+1:HEL_A_OFFSET+HEL_A_LENGTH))

% hel_beta
HEL_B_MIN = 0;
HEL_B_STEP = .01;
HEL_B_OFFSET = 92;
HEL_B_LENGTH = 10;
HEL_B = decode(HEL_B_MIN, HEL_B_STEP, Encoded(HEL_B_OFFSET+1:HEL_B_OFFSET+HEL_B_LENGTH))

% hel_gamma
HEL_G_MIN = 0;
HEL_G_STEP = .01;
HEL_G_OFFSET = 102;
HEL_G_LENGTH = 10;
HEL_G = decode(HEL_G_MIN, HEL_G_STEP, Encoded(HEL_G_OFFSET+1:HEL_G_OFFSET+HEL_G_LENGTH))

% bal_alpha
BAL_A_MIN = 0;
BAL_A_STEP = .01;
BAL_A_OFFSET = 112;
BAL_A_LENGTH = 10;
BAL_A = decode(BAL_A_MIN, BAL_A_STEP, Encoded(BAL_A_OFFSET+1:BAL_A_OFFSET+BAL_A_LENGTH))

% bal_beta
BAL_B_MIN = 0;
BAL_B_STEP = .01;
BAL_B_OFFSET = 122;
BAL_B_LENGTH = 10;
BAL_B = decode(BAL_B_MIN, BAL_B_STEP, Encoded(BAL_B_OFFSET+1:BAL_B_OFFSET+BAL_B_LENGTH))

% bal_gamma
BAL_G_MIN = 0;
BAL_G_STEP = .01;
BAL_G_OFFSET = 132;
BAL_G_LENGTH = 10;
BAL_G = decode(BAL_G_MIN, BAL_G_STEP, Encoded(BAL_G_OFFSET+1:BAL_G_OFFSET+BAL_G_LENGTH))

% temp_setpoint
TEMP_MIN = 0;
TEMP_STEP = 1;
TEMP_OFFSET = 142;
TEMP_LENGTH = 8;
TEMP = decode(TEMP_MIN, TEMP_STEP, Encoded(TEMP_OFFSET+1:TEMP_OFFSET+TEMP_LENGTH))

% control_mode
CTRL_MIN = 0;
CTRL_STEP = 1;
CTRL_OFFSET = 150;
CTRL_LENGTH = 2;
CTRL = decode(CTRL_MIN, CTRL_STEP, Encoded(CTRL_OFFSET+1:CTRL_OFFSET+CTRL_LENGTH))

% manual_adjust
MAN_ADJ_OFFSET = 152;
MAN_ADJ = decode(0, 1, Encoded(MAN_ADJ_OFFSET+1:MAN_ADJ_OFFSET+1))

% manual_select
MAN_SEL_OFFSET = 153;
MAN_SEL = decode(0, 1, Encoded(MAN_SEL_OFFSET+1:MAN_SEL_OFFSET+1))

% manual_amount
MAN_AMT_MIN = 0;
MAN_AMT_STEP = 1;
MAN_AMT_OFFSET = 154;
MAN_AMT_LENGTH = 10;
MAN_AMT = decode(MAN_AMT_MIN, MAN_AMT_STEP, Encoded(MAN_AMT_OFFSET+1:MAN_AMT_OFFSET+MAN_AMT_LENGTH))

% max velocity
VEL_MIN = 0;
VEL_STEP = .01;
VEL_OFFSET = 164;
VEL_LENGTH = 10;
VEL = decode(VEL_MIN, VEL_STEP, Encoded(VEL_OFFSET+1:VEL_OFFSET+VEL_LENGTH))
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