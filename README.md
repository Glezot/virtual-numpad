# Virtual Numpad

Emulador de teclado numérico virtual para Windows basado en el proyecto original [numpad-emulator](https://sourceforge.net/projects/numpad-emulator/).

## Características

- Configurador visual para añadir o quitar botones.
- Posibilidad de añadir cualquier tecla en cualquier posición.
- Soporte para miles de teclas y palabras completas con un solo botón.
- Repetición automática de teclas.
- Personalización de tamaños, colores, fuentes y tecla de acceso rápido para mostrar/ocultar.
- Permite introducir códigos *Alt* en cualquier dispositivo Windows.

## Capturas de pantalla

![](https://a.fsdn.com/con/app/proj/numpad-emulator/screenshots/numpadScreen1.PNG/max/max/1)
![](https://a.fsdn.com/con/app/proj/numpad-emulator/screenshots/numpadScreen3.PNG/max/max/1)

## Desarrollo con Qt Creator

1. Instala [Qt Creator](https://www.qt.io/download).
2. Abre el archivo del proyecto `src/numpad-emulator.pro`.
3. Selecciona un *kit* de compilación (MinGW o MSVC).
4. Usa **Run** o `Ctrl+R` para compilar y ejecutar.

## Crear instalador con Inno Setup

1. Compila la aplicación en modo *Release* desde Qt Creator.
2. Instala [Inno Setup](https://jrsoftware.org/isinfo.php).
3. Crea un nuevo script `.iss` indicando la ruta del ejecutable y recursos.
4. Presiona **Compile** para generar el instalador.

## Licencia

Este proyecto utiliza la licencia descrita en [license.html](license.html).

