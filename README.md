# ArduinoNano_SIM800L_8Relays
Arduino Nano + SIM800L + 8 Relays

Dudas o consultas a jesus.rebolledo.leal@gmail.com


Instrucciones de uso :

Una vez armado el circuito y programado usted puede encender el equipo y esperar alrededor de 30 seg para recibir un mensaje de exto indicando que el equipo ha encendido

Para controlar el encendido y apagado de los reles usted puede enviar los siguientes mensaje de texto al numero que tenga dentro del sim800l

Instruccion a enviar por sms        accion
On1 1234                            Enciende el rele 1   (1234 es el password definido en programacion)
On2 1234                            Enciende el rele 2   (1234 es el password definido en programacion)
On3 1234                            Enciende el rele 3   (1234 es el password definido en programacion)
On4 1234                            Enciende el rele 4   (1234 es el password definido en programacion)
On5 1234                            Enciende el rele 5   (1234 es el password definido en programacion)
On6 1234                            Enciende el rele 6   (1234 es el password definido en programacion)
On7 1234                            Enciende el rele 7   (1234 es el password definido en programacion)
On8 1234                            Enciende el rele 8   (1234 es el password definido en programacion)

Of1 1234							Apaga el rele 1   (1234 es el password definido en programacion)

Rs1 1234 							Enciende el rele 1 y al pasar 10 seg lo apaga  (1234 es el password definido en programacion)

Ontodos 1234                        Enciende todos los reles de forma secuencial

Oftodos 1234                        Apaga todos los reles de forma secuencial

Gsmok 1234                          Pregunta al equipo el estado de los reles y desvuelve un mensaje de texto con el estado de cada rele