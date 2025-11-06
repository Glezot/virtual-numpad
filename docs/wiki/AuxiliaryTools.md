# Herramientas auxiliares y utilidades

## Lanzador de personalización

Al seleccionar "Add/Remove buttons" desde el menú, `NumpadManager::slot_configure` abre dos ventanas: `AllBtnWidget` con la biblioteca de botones disponibles y `DndNumpad` para editar mediante arrastrar y soltar. Ambas se dimensionan en función de la pantalla para facilitar el trabajo simultáneo.【F:src/Numpad/NumpadManager.cpp†L488-L519】

## Editor visual (`DndNumpad`)

`DndNumpad` presenta un grid ampliado donde cada celda contiene un `ConfButton`. Permite arrastrar, copiar y cambiar el tamaño de los botones existentes, y muestra instrucciones contextuales sobre cómo combinarlo con la ventana "All buttons". Al aplicar los cambios, delega en el `NumpadManager` la actualización del archivo de configuración.【F:src/Numpad/dndnumpad.cpp†L16-L200】

## Biblioteca de botones (`AllBtnWidget`)

`AllBtnWidget` lista todas las combinaciones disponibles (`getAllBtnsConfig`) y ofrece herramientas para crear botones personalizados a partir de texto libre o códigos Alt/Unicode. Los botones generados pueden copiarse o arrastrarse al editor visual, y cualquier símbolo no soportado se reporta al usuario en la misma ventana.【F:src/Numpad/allbtnwidget.cpp†L16-L199】

## Acceso a la ayuda

Desde cualquiera de las ventanas de configuración se puede abrir la ayuda integrada. `NumpadManager::showHelp` garantiza que la ventana `HelpWindow` se cree una sola vez, se muestre en primer plano y navegue hasta la sección solicitada antes de devolver el foco al sistema.【F:src/Numpad/NumpadManager.cpp†L580-L587】
