FROM gcc

COPY . /usr/src/myapp

WORKDIR /usr/src/myapp
# -Wno-dangling-else -framework OpenGL -framework GLUT

RUN apt-get update

RUN apt-get install -y freeglut3-dev

RUN g++ window.c -o sukoGenerator -O2 -lGL -lglut

CMD ["./sukoGenerator"]