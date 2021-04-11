control:
		gcc control.c -g -o  control && ./control -i 17 -f 20 -s 3600 -t 20 -p pipe
agente:
		gcc agente.c -g -o agente && ./agente -s agente1 -a file.txt -t 20 -p pipe
clean:
		clear && rm -f *.o control agente pipe 