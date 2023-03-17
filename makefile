source = src/*.c
target = comp

$(target):	$(source)
	gcc -Wextra $(source) -o comp
	
clean:
	rm -f $(target)

