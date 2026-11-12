EL PRECIO DEL PROGRESO — V1 CALIBRADO

CONTENIDO
- web/index.html: página minimalista V1.
- arduino/el_precio_del_progreso_v1_calibrado.ino: firmware calibrado para los MG90S.
- LEEME_calibracion_servo.txt: instrucciones y seguridad.

CALIBRACIÓN INCORPORADA

Crecimiento:
0 -> 170°
1 -> 140°
2 -> 110°
2,5 -> 90°
3 -> 70°
4 -> 45°
5 -> 15°

Gini:
0,30 -> 170°
0,35 -> 143°
0,40 -> 90°
0,45 -> 45°
0,50 -> 9°

El programa interpola entre los puntos medidos. No supone una relación lineal única en todo el recorrido.

INSTALACIÓN
1. Abre el archivo .ino en Arduino IDE.
2. Selecciona Arduino Leonardo y el puerto COM correcto.
3. Cierra el Monitor Serial antes de conectar la página web.
4. Verifica y sube el firmware.
5. Ejecuta la página mediante localhost o HTTPS en Chrome/Edge. (python -m http.server 8000) en la carpeta web)
6. Pulsa "Conectar Arduino" y selecciona el puerto del Leonardo.
7. Completa una partida y pulsa "Ver resultados". La página envía crecimiento y Gini.

PROTOCOLO
La página envía una línea CSV por USB Serial a 9600 baudios:
3.60,0.385

El Arduino responde con:
OK,angulo_crecimiento,angulo_gini

SEGURIDAD ELÉCTRICA
- No alimentes dos MG90S desde el pin 5V del Arduino Leonardo.
- Usa una fuente regulada externa de 5 V, idealmente 2 A o más,
  siempre verificando las especificaciones de tus servos.
- Cable rojo de ambos servos: +5 V externo.
- Cable marrón/negro: GND de la fuente.
- Cable naranjo/amarillo: señal; crecimiento a D9 y Gini a D10.
- Une GND de la fuente externa con GND del Arduino.
- Mantén el Arduino conectado por USB al computador.
- No conectes el positivo de la fuente externa al pin 5V del Leonardo.
- Si un servo zumba, vibra, se calienta o fuerza un tope, desconecta la
  alimentación y aumenta el margen del ángulo límite antes de volver a probar.

NOTAS
La calibración corresponde a las mediciones físicas reportadas para este tablero.
Si se cambia la posición del brazo, la escala o el montaje, hay que recalibrar.
Los parámetros económicos de la página V1 siguen siendo ilustrativos/sintéticos
y no deben presentarse como resultados empíricos.
