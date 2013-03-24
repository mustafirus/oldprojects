################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Action.cpp \
../src/Application.cpp \
../src/CmdUI.cpp \
../src/Column.cpp \
../src/CreateFrame.cpp \
../src/DBCursor.cpp \
../src/DataBase.cpp \
../src/DataContext.cpp \
../src/DbException.cpp \
../src/Dict.cpp \
../src/DogFrame.cpp \
../src/Edit.cpp \
../src/EditControl.cpp \
../src/Exception.cpp \
../src/FCapts.cpp \
../src/FDC.cpp \
../src/Field.cpp \
../src/FieldFrame.cpp \
../src/Finder.cpp \
../src/Frame.cpp \
../src/FrameThread.cpp \
../src/FrameWnd.cpp \
../src/Grid.cpp \
../src/KinoApp.cpp \
../src/Log.cpp \
../src/MDIChild.cpp \
../src/MDIMain.cpp \
../src/MainWnd.cpp \
../src/OdbcCursor.cpp \
../src/OdbcDataBase.cpp \
../src/OdbcException.cpp \
../src/Pedigree.cpp \
../src/PrintAction.cpp \
../src/PrintPedigree.cpp \
../src/PrintSetup.cpp \
../src/QField.cpp \
../src/QRestr.cpp \
../src/QTable.cpp \
../src/Query.cpp \
../src/RField.cpp \
../src/RFieldControl.cpp \
../src/RKey.cpp \
../src/RLink.cpp \
../src/ROut.cpp \
../src/RSField.cpp \
../src/RSWhere.cpp \
../src/Record.cpp \
../src/RecordSet.cpp \
../src/Rib.cpp \
../src/Splash.cpp \
../src/SqlStmt.cpp \
../src/StMdfr.cpp \
../src/StatusBar.cpp \
../src/Styles.cpp \
../src/TabMap.cpp \
../src/Table.cpp \
../src/TempRecord.cpp \
../src/Thread.cpp \
../src/Wait.cpp \
../src/WinThread.cpp \
../src/Wnd.cpp \
../src/kino.cpp \
../src/stdx.cpp \
../src/user.cpp 

OBJS += \
./src/Action.o \
./src/Application.o \
./src/CmdUI.o \
./src/Column.o \
./src/CreateFrame.o \
./src/DBCursor.o \
./src/DataBase.o \
./src/DataContext.o \
./src/DbException.o \
./src/Dict.o \
./src/DogFrame.o \
./src/Edit.o \
./src/EditControl.o \
./src/Exception.o \
./src/FCapts.o \
./src/FDC.o \
./src/Field.o \
./src/FieldFrame.o \
./src/Finder.o \
./src/Frame.o \
./src/FrameThread.o \
./src/FrameWnd.o \
./src/Grid.o \
./src/KinoApp.o \
./src/Log.o \
./src/MDIChild.o \
./src/MDIMain.o \
./src/MainWnd.o \
./src/OdbcCursor.o \
./src/OdbcDataBase.o \
./src/OdbcException.o \
./src/Pedigree.o \
./src/PrintAction.o \
./src/PrintPedigree.o \
./src/PrintSetup.o \
./src/QField.o \
./src/QRestr.o \
./src/QTable.o \
./src/Query.o \
./src/RField.o \
./src/RFieldControl.o \
./src/RKey.o \
./src/RLink.o \
./src/ROut.o \
./src/RSField.o \
./src/RSWhere.o \
./src/Record.o \
./src/RecordSet.o \
./src/Rib.o \
./src/Splash.o \
./src/SqlStmt.o \
./src/StMdfr.o \
./src/StatusBar.o \
./src/Styles.o \
./src/TabMap.o \
./src/Table.o \
./src/TempRecord.o \
./src/Thread.o \
./src/Wait.o \
./src/WinThread.o \
./src/Wnd.o \
./src/kino.o \
./src/stdx.o \
./src/user.o 

CPP_DEPS += \
./src/Action.d \
./src/Application.d \
./src/CmdUI.d \
./src/Column.d \
./src/CreateFrame.d \
./src/DBCursor.d \
./src/DataBase.d \
./src/DataContext.d \
./src/DbException.d \
./src/Dict.d \
./src/DogFrame.d \
./src/Edit.d \
./src/EditControl.d \
./src/Exception.d \
./src/FCapts.d \
./src/FDC.d \
./src/Field.d \
./src/FieldFrame.d \
./src/Finder.d \
./src/Frame.d \
./src/FrameThread.d \
./src/FrameWnd.d \
./src/Grid.d \
./src/KinoApp.d \
./src/Log.d \
./src/MDIChild.d \
./src/MDIMain.d \
./src/MainWnd.d \
./src/OdbcCursor.d \
./src/OdbcDataBase.d \
./src/OdbcException.d \
./src/Pedigree.d \
./src/PrintAction.d \
./src/PrintPedigree.d \
./src/PrintSetup.d \
./src/QField.d \
./src/QRestr.d \
./src/QTable.d \
./src/Query.d \
./src/RField.d \
./src/RFieldControl.d \
./src/RKey.d \
./src/RLink.d \
./src/ROut.d \
./src/RSField.d \
./src/RSWhere.d \
./src/Record.d \
./src/RecordSet.d \
./src/Rib.d \
./src/Splash.d \
./src/SqlStmt.d \
./src/StMdfr.d \
./src/StatusBar.d \
./src/Styles.d \
./src/TabMap.d \
./src/Table.d \
./src/TempRecord.d \
./src/Thread.d \
./src/Wait.d \
./src/WinThread.d \
./src/Wnd.d \
./src/kino.d \
./src/stdx.d \
./src/user.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


