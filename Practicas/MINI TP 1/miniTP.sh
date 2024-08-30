#!/bin/bash

function mostrar_error {
    echo "Error: debe ejecutar con un parámetro al menos"
    exit 1
}

if [ -z "$1" ]; then
    mostrar_error
fi

nombre_directorio=$1
cd $HOME
mkdir -p "$nombre_directorio"
archivo="$nombre_directorio/contenido_home.txt"
ls -la > "$archivo"
cat "$archivo"
read -p "Presione Enter para terminar..."

