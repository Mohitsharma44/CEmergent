#g++ -I/usr/include/python2.7 -I/usr/include -fPIC -c -Wall -m64 -I/opt/EVT/eSDK/include/ CEmergent.cpp -o out/CEmergent.o
#g++ --shared -Wl,--export-dynamic ./out/CEmergent.o -L/usr/lib -lboost_python -L/usr/lib/python2.7/config -lpython2.7 -L/opt/EVT/eSDK/lib/  -lEmergentCamera  -lEmergentGenICam  -lEmergentGigEVision -o ./out/CEmergent.so

CC = g++

CFLAGS = -fPIC -c -Wall -m64

CUR_DIR=.
DIR_OUT=./out

# define any directories containing header files other than /usr/include
#
INCLUDES = -I/usr/include/python2.7 -I/opt/EVT/eSDK/include

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L/opt/EVT/eSDK/lib/ -L/usr/lib -L/usr/lib/python2.7/config

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -lboost_python -lpython2.7 -lEmergentCamera -lEmergentGenICam -lEmergentGigEVision

TARGET = $(DIR_OUT)/CEmergent.o $(DIR_OUT)/CEmergent.so

all: $(TARGET)

$(DIR_OUT)/%.o:  %.cpp
	mkdir -p $(DIR_OUT)
	# $< -- all the input files ;  $@ -- all the files to output
	$(CC) $(CFLAGS) $(INCLUDES) $< -o $@

$(DIR_OUT)/%.so:  $(DIR_OUT)/%.o
	$(CC) --shared -Wl,--export-dynamic $< $(LIBS) $(LFLAGS) -o $@

copyfiles:
	cp $(CUR_DIR)/*.conf $(DIR_OUT)/
	cp $(CUR_DIR)/*.py $(DIR_OUT)/
clean:
	$(RM) $(TARGET)
	$(RM) $(DIR_OUT)/*.conf
