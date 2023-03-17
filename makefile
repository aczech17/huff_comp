source = word.c dictionary.c tree_node.c node_array.c word_reader.c word_writer.c compress.c main.c
target = comp

$(target):	$(source)
	gcc -Wextra $(source) -o comp
	
clean:
	rm -f $(target)

