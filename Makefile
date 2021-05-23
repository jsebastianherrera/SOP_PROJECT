control:
		gcc control.c -g -o  control -pthread
agente:
		gcc agente.c -g -o agente 
clean:
		clear && rm -f *.o control agente  && rm pipe
