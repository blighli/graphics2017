1 #which comliler 
2 CC=g++ 
3 #where are lib filename 
4 LIB=-framework OpenGL -framework GLUT -framework Foundation 
5 OpenGL: OpenGL.o 
6 	${CC} -o OpenGL OpenGL.o ${LIB} 
7 OpenGL.o: OpenGL.cpp 
8 	$(CC) -c OpenGL.cpp -o OpenGL.o $(LIB) 
9 clean: 
10 	rm -rf *.o 
11  
