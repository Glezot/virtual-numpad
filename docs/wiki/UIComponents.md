# Componentes de la interfaz de usuario

## Ventana principal del teclado

La clase `Numpad` construye la ventana flotante con estilo de herramienta siempre visible, inicializa un `QGridLayout` para los botones y añade una barra de menú con un icono de engrane que delega en el `NumpadManager` cuando se activa.【F:src/Numpad/Numpad.cpp†L56-L118】

El widget calcula márgenes mínimos/máximos para tamaño y espacio entre botones en función de la resolución de la pantalla primaria, y mantiene un temporizador (`pm_rstrTimer`) para gestionar la repetición automática sin que la ventana obtenga el foco gracias al estilo "no activar".【F:src/Numpad/Numpad.cpp†L99-L118】

## Tipos de botones y disposición

La estructura `BtnDynamicInfo` define la forma (cuadrada, horizontal grande o vertical grande) y posición de cada botón. `Numpad::createButtons` recorre la configuración actual, combina la representación visual (`BtnStaticInfo::view`) y construye instancias específicas (`Button`, `BigHorizontalButton`, `BigVerticalButton`) según la forma deseada.【F:src/Numpad/btninfo.h†L1-L24】【F:src/Numpad/Numpad.cpp†L137-L218】

Ciertas teclas (Alt, Ctrl, Shift) se marcan como *checkable* para mantener su estado mientras el usuario compone combinaciones, y el resto queda en modo auto-repetición para soportar pulsaciones prolongadas.【F:src/Numpad/Numpad.cpp†L139-L181】

## Comportamiento de `Button`

Cada `Button` hereda de `QLabel`, acepta eventos táctiles y de ratón, y controla internamente los colores de texto y fondo para los estados presionado y reposo. Implementa un temporizador de retardo más uno de intervalo para habilitar la repetición automática y emite señales diferenciadas para pulsaciones y cambios de estado en botones conmutables.【F:src/Numpad/Buttons/Button.cpp†L26-L198】

## Indicadores y ayudas visuales

Cuando se activa el modo de códigos Alt, `createAltCodeLbl` muestra mensajes rotatorios con combinaciones útiles dentro del grid, reforzando el objetivo de introducir símbolos extendidos desde Windows.【F:src/Numpad/Numpad.cpp†L191-L197】
