
txtfile = 'BA1211-2secAUTOSCORED.txt';
zdbfile = 'BA1211.zdb';


fd = fopen(txtfile,'r');

% find the end of the file, then reset the file pointer 
fseek(fd,0,1);
eof = ftell(fd);
fseek(fd,0,-1);

% matlab fails at strings
endl = sprintf('\n');
tab = sprintf('\t');

% placeholder for the scoring keys
scores = [];

% read the fist 4 tab delimited cells from each line
% save the fourth (the scoring key) then read the rest of the line without saving it
line = cell(2,1);

c = ' '; cel = ' '; i = 1;

% loop through the file
while ftell(fd) < eof

	c = ' '; cel = ' '; i = 1;
	while c ~= endl
		c = fread(fd,1,'*char');
		if i < 3
            if c == tab 
                line{i} = cel;
                i = i+1;
                cel = ' ';
                continue;
            end
            cel = [ cel c ];    
        end
    end
    
    scores(end+1) = line{2}(1);
	fread(fd,1,'*char');
end