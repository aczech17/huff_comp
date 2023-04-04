source = src/compress.c src/dictionary.c src/main.c src/node_array.c src/tree_node.c\
src/word_reader.c src/word_writer.c src/word.c src/decompress.c src/config.c src/encrypt.c

target = comp

$(target):	$(source)
	gcc -Wextra $(source) -o $(target)
	
clean:
	rm -f $(target)

test1:	$(target)
	./comp example/papa.jpg papa.comp -O1 -v
	./comp papa.comp papa.jpg -d
	@cmp --silent example/papa.jpg papa.jpg && echo 'Są identyczne :)' || echo 'ojojoj :/'

test2: $(target)
	./comp example/tadeusz.txt tadek.comp -O2 -v ifiwerearichman
	./comp tadek.comp tadek.txt -d ifiwerearichman
	@cmp --silent example/tadeusz.txt tadek.txt && echo 'Są identyczne :)' || echo 'ojojoj :/'
