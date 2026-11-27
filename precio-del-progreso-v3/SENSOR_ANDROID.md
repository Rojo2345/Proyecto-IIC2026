# Sensor Android y esfuerzo progresivo virtual

La simulación principal sigue en `public/index.html`. La página `public/sensor.html` lee la orientación del Android y envía las lecturas al servidor Next.js mediante `/api/sensor`.

## Comportamiento de los dos ejes

- **Crecimiento:** la inclinación adelante/atrás se traduce directamente a un valor de crecimiento respecto del centro calibrado. Al volver al centro, el crecimiento vuelve al valor inicial seleccionado en el computador.
- **Desigualdad (Gini):** ya no se asigna directamente al ángulo. Es un valor acumulativo. Inclinar hacia el lado configurado como igualdad lo reduce poco a poco; la reducción se ralentiza cuanto más bajo es el Gini. Inclinar al lado contrario lo aumenta más rápido.
- **Posición neutra:** al volver al centro, el Gini deja de cambiar, pero conserva el valor alcanzado. No hay retorno automático.
- En la página Android se puede invertir qué signo de gamma representa igualdad, por si la orientación física del teléfono queda al revés.

Los parámetros iniciales de la curva están en `public/sensor.html`: `GINI_EQUALITY_RATE`, `GINI_INEQUALITY_RATE`, `GINI_DEAD_ZONE_DEG` y `GINI_INTENSITY_EXPONENT`. Son valores de prototipo que se pueden calibrar después de probar el teléfono real.

## Ejecutar en Linux

1. Descomprime la actualización dentro de la carpeta del proyecto, conservando la estructura `public/` y `app/`.
2. En una terminal, desde la carpeta del proyecto, ejecuta `pnpm install` si todavía no has instalado dependencias.
3. Inicia Next.js con `pnpm dev`.
4. En otra terminal, inicia un túnel HTTPS temporal: `cloudflared tunnel --url http://localhost:3000`.
5. Abre `http://localhost:3000` en el computador. Selecciona **Celular Android**.
6. En el teléfono abre la URL HTTPS generada, seguida de `/sensor.html`. Pulsa **Activar sensor** y luego **Calibrar centro**.
7. Prueba la inclinación adelante/atrás para crecimiento. Para Gini, mantén la inclinación hacia el lado de igualdad para reducirlo; la reducción será cada vez más lenta. Inclina al lado contrario para aumentarlo más rápido. Vuelve al centro para detener el cambio sin deshacerlo.

## Seguridad y límites

- La API de orientación requiere HTTPS (salvo excepciones locales del navegador).
- Quick Tunnel publica temporalmente el servidor de desarrollo en Internet. No compartas la URL y cierra el túnel con `Ctrl+C` al terminar.
- La ruta API conserva el último dato en memoria del proceso Next.js. Está pensada para una demostración local con un único servidor de desarrollo.
- `beta` y `gamma` son la asignación inicial de ejes. La orientación exacta depende de cómo se monte el teléfono; si el sentido de igualdad queda invertido, cambia la opción en la página Android.
- El Arduino, los servomotores y cualquier resistencia mecánica quedan fuera de esta etapa: la dificultad se representa virtualmente mediante la tasa acumulativa de cambio del Gini.
