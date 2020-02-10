################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ChessAnalyse.cpp \
../src/ChessBoard.cpp \
../src/ChessCom.cpp \
../src/Game.cpp \
../src/Parser.cpp \
../src/ProcessInOut.cpp \
../src/TextInput.cpp \
../src/ThreadSaveQueue.cpp 

OBJS += \
./src/ChessAnalyse.o \
./src/ChessBoard.o \
./src/ChessCom.o \
./src/Game.o \
./src/Parser.o \
./src/ProcessInOut.o \
./src/TextInput.o \
./src/ThreadSaveQueue.o 

CPP_DEPS += \
./src/ChessAnalyse.d \
./src/ChessBoard.d \
./src/ChessCom.d \
./src/Game.d \
./src/Parser.d \
./src/ProcessInOut.d \
./src/TextInput.d \
./src/ThreadSaveQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


