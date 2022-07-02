# PNG  to ledstrip

  **contenido:**
  
  - PNG_to_Arduino_Flash.py:
  
    Utilidad python que nos permite convertir imagenes en formato 'png' en su matriz de componentes de color RGB 
    para ser representadas en pantallas led basadas en tiras de leds direccionables.
    Genera un fichero 'fotogramas.h' en la carpeta donde se encuentra el fichero '.py'
    que contiene los datos de color para ser alojados en la flash de Arduino

    Se pueden usar imagenes .jpg con solo especificarlo en el codigo, pero se recomienda que sean imagenes '.png' ya que de esa forma
    no habra variaciones entre el fichero que generemos en nuestro programa de edicion y el resultado almacenado.
  
  - PNG2LedStrip_v1.0:
  
    Carpeta con el codigo Arduino para representar la informacion de 'fotogramas.h' en pantallas led de 16x16 pixles
  
  - fotogramas:
  
    Carpeta con un set de imagenes(iconos) de ejemplo. Contiene un fichero erroneo para ilustar que l programa pyhon
    descarta aquellos ficheros que no son validos o que no cumplen con las especificadiones de 16x16
  
  - pixelformer.exe:
  
    utilidad freeware para editar/crear iconos que podemos exportar a png y usar en nuesta pantalla
