source = src/compress.c src/dictionary.c src/main.c src/node_array.c src/tree_node.c src/word_reader.c src/word_writer.c src/word.c
target = comp

$(target):	$(source)
	gcc -Wextra $(source) -o comp
	
clean:
	rm -f $(target)

