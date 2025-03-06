echo "$#"
if [ $# -eq 1 ]; then
    echo "building ur project : $1 ..... (sber chwya)"

    gcc -Wall -Wformat=0 $(pkg-config --cflags gtk+-3.0) -w -o main $1 $(pkg-config --libs gtk+-3.0) && ./main.exe
else
    echo "building ur project ..... (sber chewya)"
    gcc -Wall -Wformat=0 $(pkg-config --cflags gtk+-3.0) -w -o main main.c $(pkg-config --libs gtk+-3.0) && ./main.exe
fi
    
#!/bin/bash

# gcc $(pkg-config --cflags gtk+-3.0) -w -o main main.c $(pkg-config --libs gtk+-3.0) && ./main.exe



