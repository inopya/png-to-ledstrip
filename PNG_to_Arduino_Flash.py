#!/usr/bin/python
# -*- coding: utf-8 -*-


#       _\|/_   A ver..., ¿que tenemos por aqui?
#       (O-O)        
# ---oOO-(_)-OOo---------------------------------
 
 
##########################################################
# ****************************************************** #
# *             ARDUINO PARA PRINCIPIANTES             * #
# *         Incluir imagenes en flash arduino          * #
# *                  Autor:  inopya                    * #
# *                                                    * #
# *     PictureToMemory  v1.0   Fecha: 18/08/2020      * #
# *           ultima revision   Fecha: 25/06/2022      * #
# ****************************************************** #
##########################################################

'''
    Utilidad que nos permite convertir imagenes 'png'
    en su matriz de componentes de color RGB para ser representadas en
    pantallas led basadas en tiras de leds direccionables
    Genera un fichero '.h' en la carpeta donde se encuentre este fichero
    que contiene los datos de color para ser alojados en la flash de Arduino

    revision 25/06/2022  para adaptarla a pantallas 16x16

    se pueden usar imagenes .jpg con solo especificarlo en el codigo,
    pero se recomienda que sean imagenes '.png' ya que de esa forma
    no habra variaciones entre el fichero que generemos
    en nuestro programa de edicion y el resultado almacenado.

    
    SOLO PARA PYTHON 3.x

    algunas bibliotecas que quizas necesites instalar:

    python -m pip install opencv-python
    python -m pip install numpy

    (si en linux no puedes instalar directamente, intentalo mejor con 'sudo'
    y si no te funciona con 'pip install' intentalo con 'pip3 install')
    
'''




#====================================================================================================
#  IMPORTACION DE BIBLIOTECAS
#====================================================================================================

import os               #manejo de funciones del sistema operativo 
from os import walk     #funciones para movernos por directorios


#Importar librería cv2
import cv2



#====================================================================================================
#  FUNCIONES
#====================================================================================================

def listarFicheros(ruta = '.'):
    directorio, subdirs, archivos = next(walk(ruta))
    return archivos






#====================================================================================================
#  PROGRAMA
#====================================================================================================
#Ruta absoluta en la que se encuentra el script. Util para las llamadas desde el inicio del sistema
RUTA_PROGRAMA = os.path.dirname(os.path.abspath(__file__)) +'/'
ruta = os.getcwd() +'/'


#podemos activar esta bandera y usar el patron para seleccionar solo los ficheros
#que contengan determinados caracteres en su nombre
FLAG_usar_patron_busqueda = False
PATRON_BUSQUEDA = 'img'

#carpeta que contiene las imagenes (directorio junto al fichero python)
carpeta_imagenes  = 'fotogramas' 

RUTA_TRABAJO = RUTA_PROGRAMA + carpeta_imagenes + '/'

#lista de ficheros en el direcctorio seleccionado. Puede contener ficheros de cualquier tipo
lista = listarFicheros(RUTA_TRABAJO)

 
#iterar sobre la lista general de ficheros para localizar los ficheros de imagen validos
listaFicheros = [] 

for fichero in lista:
    (nombreFichero, extension) = os.path.splitext(fichero)
    
    if(FLAG_usar_patron_busqueda):
        if(nombreFichero.find(PATRON_BUSQUEDA)==-1):
            continue    
            
    if(extension == ".png"):
        rutaFichero = RUTA_TRABAJO + nombreFichero + extension
        listaFicheros.append(rutaFichero)


#crear el fichero para contener los datos de los fotogramas
outfile = open("fotogramas.h", 'w')
outfile.write("/* Codigo Generado por: inopya PictureToMemory  v1.0 */ \n\n")

array_fotogramas = "const uint8_t* const fotogramas[] PROGMEM = {\n\t\t"

#procesar la lista de ficheros de imagen para generar las matrices de datos
contador=0
errores=0
for f in range (0,len(listaFicheros)):
    try:
        if (f<10):
            numeroFotograma="0"+str(f)
        else:
            numeroFotograma=str(f)
        
        bloque_fotograma="const uint8_t f_" + numeroFotograma + "[] PROGMEM = {\n"

        #Leer imagen
        img = cv2.imread(listaFicheros[f])
        
        dimensiones = img.shape # (alto, ancho, bytes x pixel)

        alto = dimensiones[0]
        ancho = dimensiones[1]
        profundidad_color = dimensiones[2]
        print("(", alto,"x",ancho,")  ", listaFicheros[f].split("/")[-1] ) 
        
        #evitar procesar imagenes que no conicidan con las dimensiones de a pantalla
        if(alto!=16 or ancho!=16):
            continue
            
        for y in range(0,alto):
            linea="" # filas de nuestra imagen de arriba a abajo
            for x in range(0,ancho):
                color_pixel=img[y,x] #fila,columna
                #OpenCV trata los datos en formato  BGR por tanto invertimos el orden de las componentes
                componentesColor = str(color_pixel[2])+","+str(color_pixel[1])+","+str(color_pixel[0])
                linea+=(str(componentesColor)+",")         
            bloque_fotograma+=("\t\t"+linea+"\n")
        bloque_fotograma+="\t};\n\n"
        outfile.write(bloque_fotograma)
        contador+=1
        
        #si se ha procesado el fichero, añadirlo al array de fotograma
        array_fotogramas += ("f_" + numeroFotograma + ", ")
        if((f+1)%10==0):
            array_fotogramas+="\n\t\t"

    except:
        print(" ERROR procesando imagen: ", listaFicheros[f].split("/")[-1])
        errores+=1


#añadir el array con la lista de todos los fotogramas generados
if(contador>0):
    outfile.write(array_fotogramas)
    outfile.write("\n\t};")
    #fin proceso correcto
    print("\n\nProceso finalizado >>> Generados "+str(contador)+" fotogramas")
    if (errores>0):
        if (errores==1):
            print("\t("+str(errores)+") imagen contenia errores y se descarto")
        else:
            print("\t("+str(errores)+") imagenes contenian errores y se descartaron")
    print("\nDatos guardados en el fichero: \'fotogramas.h\'")        
    print("Recuerda incluir los datos en tu codigo Arduino con:\n  #include \"fotogramas.h\"\n\n")
else:
    outfile.write("\n//\t ERROR GENERANDO INFORMACION, NINGUN FICHERO VALIDO")
    #fin proceso no valido
    print("\n\nImposible generar datos >>> Fichero \'fotogramas.h\' sin informacion")

#cerrar fichero y dejarlo listo para su uso en Arduino
outfile.close()




