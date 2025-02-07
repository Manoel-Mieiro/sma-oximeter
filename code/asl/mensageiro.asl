/* Initial beliefs and rules */
network(["Lucy Steel", "Diego Brando", "Funny Valentine", "Gyro Zepelli"]).
patient("Jonny Joestar").

/* Initial goals */


/* Plans */
+!kqml_received(Sender, tell, ST, MsgId) <- 
                                .print("Quadro de gravidade ", ST, " recebido de ", Sender);
				.print("Adicionando severidade do caso a base de beliefs...");
				.wait(2000);
				+severity(S);
				.print("Severidade Armazenada! Notificando rede de apoio...").