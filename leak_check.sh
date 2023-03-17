valgrind -s --leak-check=full --show-leak-kinds=all ./comp example/3 tempf -O1 > /dev/null
rm tempf
