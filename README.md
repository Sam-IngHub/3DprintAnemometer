# 3DprintAnemometer
Diseño de un anemómetro de coperolas configurado con Arduino. Un sensor óptico LM393 mide las vueltas por segundo del eje y mediante un factor de conversión mide la velocidad en m/s. La calibración fue realizada con ayuda de un anemómetro de hilo caliente midiendo la misma corriente de aire en un túnel de viento.
Fue realizado para medir las velocidades del flujo dentro de un túnel de viento, pero puede ser colocado en un balcón. El modelo de anemómetro se puede descargar por Thingiverse: "Anemometer by DigitalUrban"
Van a haber dos códigos, el normal donde el procesador esté conectado a la computadora por USB, y otro donde los valores se mostrarán en un display LCD 16*2.

1/8/2026
--> Se debe hacer aparte una base y agregar un hilo para que pueda notar el sensor. La mejora a futuro será modificar la base del archivo en CAD para que tenga un espacio para encastrar el sensor, así es solo imprimir y ensamblar.
--> No está calibrado, se va a calibrar en el túnel de viento con herramientas de precisión estos días.

Una idea a futuro es añadir un sensor de temperatura, también sería interesante conectarlo a una red wifi para tener los valores de viento en tu celular.


++ PASOS PARA CONFIGURARLO ++
- Imprimir la figura, conseguir el sensor LM393 con el display LCD y realizar el cableado (tacómetro/medidor de Hz)
- Carga el archivo que te sirva a tu Arduino.
- Si dispones de medidores previamente calibrados, compara las velocidades obtenidas. Si es necesario, cambia el valor de 'k' hasta que las dos mediciones coincidan.
