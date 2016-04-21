build: quadtree.c
	gcc quadtree.c -g -Wall -o quadtree

run: quadtree
	./quadtree test1.bmp test1-out.bmp

clean:
	rm quadtree *out.bmp
