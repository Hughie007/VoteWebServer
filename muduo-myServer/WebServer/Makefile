APPNAME=ServerMain
CXX:=g++
CFLAGS:= -g
LIBS:= -lpthread -lmysqlclient
all:
	${CXX} ${CFLAGS} *.cpp ./TcpConnection/*.cpp ./TimeManager/*.cpp ./syncTools/*.cpp ./Reactor/*.cpp ./Pool/*.cpp ./Log/*.cpp -o ${APPNAME} $^ ${LIBS}
clean:
	rm -rf ${APPNAME}
