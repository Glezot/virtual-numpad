# Sistema de configuración y persistencia

## Preferencias de interfaz

`NumpadManager` guarda ajustes visuales y de comportamiento mediante `QSettings`, incluyendo la visibilidad del menú contextual, la presencia del botón de cambio de diseño y la última posición conocida de la ventana. Las llamadas de escritura se agrupan en métodos como `writeMenuVisibleToSettings`, `writeLayoutBtnVisibleToSettings` y `writeNumpadPosition`, mientras que `readNumpadPosition` decide si recuperar la posición almacenada o la inicial en función de la preferencia "Recordar última posición".【F:src/Numpad/NumpadManager.cpp†L772-L843】

También persiste la posición inicial manual, el estado "recordar" y valida las coordenadas contra las pantallas disponibles para evitar que el teclado aparezca fuera de la vista.【F:src/Numpad/NumpadManager.cpp†L846-L974】

## Configuración desde la UI

El `SettingsDialog` ofrece controles para alternar el menú del engrane, mostrar el selector de diseño, activar el inicio automático con Windows, gestionar la posición inicial y elegir la tecla global que muestra/oculta el teclado, siempre que el *hook* global se haya instalado correctamente.【F:src/Numpad/SettingsDialog.cpp†L69-L167】

También expone deslizadores para tamaño y espaciado de botones, junto con selectores de color y tipografía que delegan en el `Numpad` para aplicar los cambios y que luego son almacenados por el `NumpadManager`.【F:src/Numpad/SettingsDialog.cpp†L168-L199】【F:src/Numpad/NumpadManager.cpp†L423-L441】

## Definiciones estáticas de botones

El método `loadBtnsStaticInfo` crea un mapa de `BtnStaticInfo` que asocia cada identificador con su representación HTML y códigos de teclado/Alt correspondientes. La lista incluye dígitos numéricos, símbolos especiales, teclas de función y variantes alfanuméricas para generar palabras completas mediante códigos Alt.【F:src/Numpad/NumpadManager.cpp†L1120-L1260】

## Tecla global de activación

`setShowHideKey` busca en la colección pre-cargada de teclas rápidas y, si el *hook* está operativo, actualiza los atajos de menú para reflejar la tecla seleccionada. Esto permite que la misma tecla sirva tanto para alternar desde el sistema como desde la interfaz.【F:src/Numpad/NumpadManager.cpp†L430-L444】
