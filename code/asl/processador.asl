/* Initial beliefs and rules */

/* Initial goals */

/* Plans */

/* Aqui e recebida a percept de SPo2 vindo do monitor e adiciona spo2 a base de beliefs*/
+!kqml_received(Sender, tell, S, MsgId) <- 
                                .print("Taxa SPo2 recebida de ", Sender, ": ", S, "%");
                                /* Belief Update */
    				            -spo2(processador, _);   
                                +spo2(processador, S);   
				.wait(4000);
				!saturationAssess;
				.print("Aguardando diagnostico...");
				.wait(6000);
				!notifyNet.

/*
Valores de SpO2 
97-99%: Bom nivel de oxigenio no sangue
94-96%: Nivel medio de oxigenio no sangue
90-93%: Baixo nivel de oxigenio no sangue
Abaixo de 92%: Sinais de insuficiencia respiratoria muito grave
Abaixo de 90%: Indicativo de emergencia medica
*/

/*  Classificando o SPO2  */

+!saturationAssess 
    : spo2(processador, S) & S >= 96  <- 
    .print("SpO2: ", S, "%. Status: NORMAL - Bom nivel de oxigenio no sangue.");
    -statusSaturation(_);
    +statusSaturation("NORMAL").

+!saturationAssess
    : spo2(processador, S) & S >= 90 & S < 96 <- 
    .print("SpO2: ", S, "%. Status: ALERTA - Nivel medio de oxigenio no sangue.");
    -statusSaturation(_);
    +statusSaturation("ALERTA").
    
+!saturationAssess 
    : spo2(processador, S) & S < 90 <- 
    .print("SpO2: ", S, "%. Status: CRITICO - Emergencia medica!");
    -statusSaturation(_);
    +statusSaturation("CRITICO").


+!notifyNet: statusSaturation(ST) & ST = "CRITICO" <- 
.print("EMERGENCIA! Notificando rede de apoio...");
.wait(2000);
.send(mensageiro, tell, ST).
    
+!notifyNet: statusSaturation(ST) & ST = "ALERTA" <- 
.print("O quadro de alerta. Manter em observacao.").

+!notifyNet: statusSaturation(ST) & ST = "NORMAL" <- 
.print("Paciente esta com niveis normais de SPo2 no sangue. Manter em observacao.").

-!notifyNet <- .print("Falha ao executar notifyNet").