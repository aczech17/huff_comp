source = src/compress.c src/dictionary.c src/main.c src/node_array.c src/tree_node.c src/word_reader.c src/word_writer.c src/word.c src/decompress.c
target = comp

$(target):	$(source)
	gcc -Wextra $(source) -o $(target)
	
clean:
	rm -f $(target)

