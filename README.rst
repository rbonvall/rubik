Cubo rubik
==========

Compilacion y ejecucion
-----------------------
Para compilar::

    $ make

Para ejecutar::

    $ ./tarea5


Descripcion del programa
------------------------
El programa es un simulador de un cubo rubik.  Permite simular absolutamente
todas las funcionalidades de un cubo rubik de verdad.


Manipulacion de la camara
-------------------------
La camara puede trasladarse alrededor del cubo, subir para ver la cara de
arriba o bajar para ver la cara de abajo.  Las posiciones finales de la camara
estan predeterminadas, por lo que el movimiento no es libre.  Las traslaciones
se activan con las teclas::

      w
    a s d


* a:  girar a la izquierda
* d:  girar a la derecha
* w:  subir la camara
* s:  bajar la camara

Para simplificar la implementacion, la camara se mueve en linea recta entre los
puntos predeterminados.  Esto entrega la impresion de que el cubo "salta" hacia
el observador.  Este efecto no apareceria si la camara orbitara circularmente
alrededor del cubo.


Manipulacion del cubo
---------------------
Nueve letras son utilizadas para manipular el cubo, cada una asignada a una
"rebanada" del cubo.  Las mayúsculas rotan la rebanada en un sentido, y las
minúsculas en otro::


                     / \
                    /   \
                  / \   / \
                 /   \ /   \
               / \   / \   / \
              /   \ /   \ /   \
        F f  |\   / \   / \   /|
             | \ /   \ /   \ / |
        G g  |\ |\   / \   /| /|
             | \| \ /   \ / |/ |
        H h  |\ |\ |\   /| /| /|
             | \| \| \ / |/ |/ |
              \ |\ |\ | /| /| /
               \| \| \|/ |/ |/
              Z  \ |\ | /| /   N
              z   \| \|/ |/    n
                 X  \ | /   B
                 x   \|/    b
                    C    V
                    c    v


Este diagrama esta visto desde la posicion inicial de la camara.  Cuando se
mueve la camara alrededor del cubo, hay que tener cuidado porque la posicion
relativa entre las teclas y el observador cambia.


Manipulacion de las luces
-------------------------
Hay dos luces:  una en (4, 4, 4) y la otra en -(4, 4, 4).  Las teclas 1 y 2
sirven de interruptores para ellas, respectivamente.


Respecto a los requerimientos de la tarea
-----------------------------------------
No se implemento movimiento y transformaciones de luces porque no tenia mucho
sentido para el juego que se decidio desarrollar.  Para compensar por esta
falta, se incorporo texturas al cubo, para demostrar una funcionalidad no
requerida pero que fue aprendida en el ramo.


Observacion
-----------
El programa fue probado en un computador lento, y las animaciones estan
implementadas de modo que se vean bien en el.  Quizas en un computador rapido
las animaciones no se vean bien.  En este caso, conviene dar un valor mas alto
a las constantes ``ROTATION_STEPS`` en ``main.c`` y ``TRASLATION_STEPS`` en ``camera.h``.

