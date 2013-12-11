.SUFFIXES       : .cpp .o

CC              = gcc
CXX             = $(CC)

#########################################
# Define / Compile flags / Linking flags #
#----------------------------------------#
DEFINE          =

CFLAGS          = -g -Wall -fPIC -Wno-sign-compare -Wno-multichar
LFLAGS          = -lstdc++ -ldl -lpthread 

############################
# Main Sources and Objects #
# (Edit your TARGET name.) #
#--------------------------#
SRCS            = $(subst $(UTSRC),,$(wildcard *.cpp))
OBJS            = $(SRCS:.cpp=.o)
TARGET          = RPCAM
INSTALLS        = $(TARGET)

##################################################
# Linking Part                                   #
# (Select linking option on your TARGET purpose) #
#------------------------------------------------#
all				: $(INSTALLS)
$(TARGET)		: $(OBJS)
				@$(CXX) -o $(TARGET) $(OBJS) $(LFLAGS)
				@echo "==============================================================="
				@echo "[35m$(TARGET) Compile Complete !![0m"

.cpp.o			:
				@echo "[$(TARGET)] Compiling [33m$<[0m ..."
				@$(CC) $(CFLAGS) -c $< -o $@ 

clean			:
				@echo "[$(TARGET)] Delete Object Files !!"
				@$(RM) $(OBJS) $(TARGET) $(UTOBJ) $(UTBIN)