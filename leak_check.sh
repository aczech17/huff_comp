valgrind -s --leak-check=full --show-leak-kinds=all ./comp example/tadeusz.txt tadeusz.comp -O1 ifiwerearichman
valgrind -s --leak-check=full --show-leak-kinds=all ./comp tadeusz.comp tadeusz.txt -d ifiwerearichman
rm tadeusz.comp
rm tadeusz.txt
