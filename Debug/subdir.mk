################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AMFunction.cpp \
../Parser.cpp \
../SmallBasicSet.cpp \
../Test.cpp \
../tests.cpp \
../toolbox.cpp 

OBJS += \
./AMFunction.o \
./Parser.o \
./SmallBasicSet.o \
./Test.o \
./tests.o \
./toolbox.o 

CPP_DEPS += \
./AMFunction.d \
./Parser.d \
./SmallBasicSet.d \
./Test.d \
./tests.d \
./toolbox.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


