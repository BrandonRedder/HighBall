function Message = EncodeMessage()
% for testing purposes this creates a random binary signal


drops = 15;
drops_bin = int2bin(drops);
message = false(1,80);
message(11:20) = true(1,10);
message(end-length(drops_bin)+1:end) = drops_bin;

bin = message;

temp = datetime('now','TimeZone','local');
date = datestr(temp, 'yy-MM-dd_HH-mm-ss');
file_name = [date,'data.sbd'];
FID = fopen(file_name, 'w+');
fwrite(FID, bin, 'ubit1');
fclose('all');
end

function bin = int2bin(integer)
l = ceil(log2(integer));
integer = integer - 2^l;
bin = true(1,l);
for val = l-1:-1:1
    if (integer < 2^val)
        bin(val) = false;
    else
        integer = integer - 2^val;
    end
end
end