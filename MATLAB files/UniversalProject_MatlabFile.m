s = serial('COM2','baudrate', 115200);
fopen(s);
for i = 1:120
    fwrite(s, 'A');fwrite(s, i);fwrite(s, 'B');fwrite(s,(120 - i));fwrite(s, 'C');fwrite(s, ((i*2)-40)); pause(0.1);
end
fwrite(s, 'A');fwrite(s, 0);fwrite(s, 'B');fwrite(s, 0);fwrite(s, 'C');fwrite(s, 0);
fclose(s);
disp('Process Completed');