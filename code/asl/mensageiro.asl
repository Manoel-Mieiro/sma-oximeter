/* Initial beliefs and rules */
network(["Lucy Steel", "Diego Brando", "Funny Valentine", "Gyro Zepelli"]).
patient("Jonny Joestar").

+!kqml_received(Sender, tell, ST, MsgId) <- 
    .print("Quadro de gravidade ", ST, " recebido de ", Sender);
    .print("Adicionando severidade do caso a base de beliefs...");
    .wait(2000);
    +severity(ST);
    .print("Severidade Armazenada! Notificando rede de apoio...");
    ?network(List);
    ?patient(Patient);   // Adiciona uma consulta para obter o paciente
    !alertNet(List, Patient).

+!alertNet([], P) <- 
    .print("Rede de apoio completamente notificada!").

+!alertNet([Member | Left], P) <- 
    .print(Member, ", favor procurar pelo paciente ", P);
    .print("Caso confirmado de hipoxemia");
    !alertNet(Left, P).
