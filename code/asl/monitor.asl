/* beliefs and rules */
/* serialPort(ttyUSB0). */
/* serialPort(ttyEmulatedPort0). */

serialPort(ttyUSB0).

/* Initial goals */
!connect.

/* Achievement Plans */

+!connect: 
serialPort(Port) <- 
	.argo.port(Port); 
	.argo.limit(500); 
	.argo.percepts(open).

+!infoLCD(M)  <- .argo.act(M).
+!yellowAlert <- .argo.act(yellowOn).
+!greenAlert  <- .argo.act(greenOn).
+!redAlert    <- .argo.act(redOn).
+!blueAlert   <- .argo.act(blueOn).

/* Belief Plans */
+device(D): 
D = arduinoWithLCDKeypadShield <- 
	.my_name(N); 
	.concat(N," is online!",LCDMessage);
	!infoLCD(LCDMessage);
	.wait(1000);
	!redAlert.

+port(Port,Status):
Status = off | Status = timeout <-
	.argo.percepts(close);
	.print("Serial port ",Port," is ",Status);
	.print("Stopping the MAS");
	.stopMAS.

+spo2Rate(S) <- .wait(10000);
		.print("New perception-> SPo2 rate's: ",S," %");
		.wait(2000);
		.print("Enviando taxa para processador...");
		.wait(2000);
		.send(processador, tell, S);
		.wait(2000);
		.print("Saturacao eviada com sucesso."). 