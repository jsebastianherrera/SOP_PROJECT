control:
		gcc control.c -g -o  control
agente:
		gcc agente.c -g -o agente
clean:
		clear && rm -f *.o control agente pipe
