# Arquitectura y ciclo de vida

## Punto de entrada

La ejecución comienza en `main.cpp`, donde se inicializa la instancia de `QApplication`, se crea un `SingleInstanceGuard` para evitar múltiples procesos simultáneos y se conecta la señal de activación al `NumpadManager` antes de iniciar el bucle principal de Qt.【F:src/Numpad/main.cpp†L21-L39】

## Control de instancia única

`SingleInstanceGuard` utiliza un `QLocalServer` para reclamar un nombre de servidor exclusivo. Si otra instancia intenta ejecutarse, se conecta al servidor existente, envía un mensaje `activate` y provoca que la instancia principal emita la señal `activationRequested`. El destructor elimina el servidor para liberar el recurso cuando la aplicación finaliza.【F:src/Numpad/SingleInstanceGuard.cpp†L1-L82】【F:src/Numpad/SingleInstanceGuard.h†L1-L32】

## Rol del `NumpadManager`

`NumpadManager` actúa como orquestador general: configura iconos, menús y diálogo de ajustes, inicializa las configuraciones por defecto y carga tanto la información estática de botones como la disposición actual desde archivo. También instala el *hook* global del teclado cargando dinámicamente `KeyboardHook.dll` y registrando un protocolo personalizado para la comunicación con el sistema.【F:src/Numpad/NumpadManager.cpp†L65-L158】

Además, escucha mensajes nativos de Windows mediante `nativeEvent`. Allí intercepta el mensaje personalizado `KEYBOARDHOOKMSG` para invocar `slot_showHideNumpad` cuando se detecta la tecla configurada, y responde a `WM_QUERYENDSESSION` cerrando la aplicación de forma ordenada.【F:src/Numpad/NumpadManager.cpp†L604-L624】【F:src/Numpad/KeyboardHookMsg.h†L21-L26】

## Ciclo de visibilidad y foco

Las peticiones para mostrar u ocultar el teclado virtual terminan en `slot_showHideNumpad`, que sincroniza el estado del menú contextual y alterna la ventana entre visible, minimizada u oculta manteniendo el estilo "no activar". Cuando se requiere llevar la aplicación al frente (por ejemplo, tras recibir `activationRequested`), `bringToForeground` restaura la ventana, la enfoca y asegura que se muestre sobre otras aplicaciones.【F:src/Numpad/NumpadManager.cpp†L452-L472】【F:src/Numpad/NumpadManager.cpp†L888-L916】
