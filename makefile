source = word.c dictionary.c tree_node.c node_array.c main.c
target = comp

$(target):	$(source)
	gcc -Wextra $(source) -o comp
	
clean:
	rm -f $(target)

