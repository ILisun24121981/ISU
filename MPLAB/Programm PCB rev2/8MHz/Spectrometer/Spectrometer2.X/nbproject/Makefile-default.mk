#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../../adc.c ../../cn.c ../../crc.c ../../GasExtractSensor.c ../../GasPlasmaFlowSensor.c ../../GasPressureSensor.c ../../GasTransportFlowSensor.c ../../interruptpriority.c ../../main.c ../../memory.c ../../PMTSensor.c ../../Pneumatics.c ../../pwm.c ../../SM1150.c ../../timer.c ../../wake.c ../../StepMotorProba.c ../../StepMotorPMT.c ../../US.c ../../uart.c ../../packet.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/43898991/adc.o ${OBJECTDIR}/_ext/43898991/cn.o ${OBJECTDIR}/_ext/43898991/crc.o ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o ${OBJECTDIR}/_ext/43898991/interruptpriority.o ${OBJECTDIR}/_ext/43898991/main.o ${OBJECTDIR}/_ext/43898991/memory.o ${OBJECTDIR}/_ext/43898991/PMTSensor.o ${OBJECTDIR}/_ext/43898991/Pneumatics.o ${OBJECTDIR}/_ext/43898991/pwm.o ${OBJECTDIR}/_ext/43898991/SM1150.o ${OBJECTDIR}/_ext/43898991/timer.o ${OBJECTDIR}/_ext/43898991/wake.o ${OBJECTDIR}/_ext/43898991/StepMotorProba.o ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o ${OBJECTDIR}/_ext/43898991/US.o ${OBJECTDIR}/_ext/43898991/uart.o ${OBJECTDIR}/_ext/43898991/packet.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/43898991/adc.o.d ${OBJECTDIR}/_ext/43898991/cn.o.d ${OBJECTDIR}/_ext/43898991/crc.o.d ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d ${OBJECTDIR}/_ext/43898991/interruptpriority.o.d ${OBJECTDIR}/_ext/43898991/main.o.d ${OBJECTDIR}/_ext/43898991/memory.o.d ${OBJECTDIR}/_ext/43898991/PMTSensor.o.d ${OBJECTDIR}/_ext/43898991/Pneumatics.o.d ${OBJECTDIR}/_ext/43898991/pwm.o.d ${OBJECTDIR}/_ext/43898991/SM1150.o.d ${OBJECTDIR}/_ext/43898991/timer.o.d ${OBJECTDIR}/_ext/43898991/wake.o.d ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d ${OBJECTDIR}/_ext/43898991/US.o.d ${OBJECTDIR}/_ext/43898991/uart.o.d ${OBJECTDIR}/_ext/43898991/packet.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/43898991/adc.o ${OBJECTDIR}/_ext/43898991/cn.o ${OBJECTDIR}/_ext/43898991/crc.o ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o ${OBJECTDIR}/_ext/43898991/interruptpriority.o ${OBJECTDIR}/_ext/43898991/main.o ${OBJECTDIR}/_ext/43898991/memory.o ${OBJECTDIR}/_ext/43898991/PMTSensor.o ${OBJECTDIR}/_ext/43898991/Pneumatics.o ${OBJECTDIR}/_ext/43898991/pwm.o ${OBJECTDIR}/_ext/43898991/SM1150.o ${OBJECTDIR}/_ext/43898991/timer.o ${OBJECTDIR}/_ext/43898991/wake.o ${OBJECTDIR}/_ext/43898991/StepMotorProba.o ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o ${OBJECTDIR}/_ext/43898991/US.o ${OBJECTDIR}/_ext/43898991/uart.o ${OBJECTDIR}/_ext/43898991/packet.o

# Source Files
SOURCEFILES=../../adc.c ../../cn.c ../../crc.c ../../GasExtractSensor.c ../../GasPlasmaFlowSensor.c ../../GasPressureSensor.c ../../GasTransportFlowSensor.c ../../interruptpriority.c ../../main.c ../../memory.c ../../PMTSensor.c ../../Pneumatics.c ../../pwm.c ../../SM1150.c ../../timer.c ../../wake.c ../../StepMotorProba.c ../../StepMotorPMT.c ../../US.c ../../uart.c ../../packet.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=24FJ128GA106
MP_LINKER_FILE_OPTION=,--script="..\..\APP 24FJ128GA106 IVT remap.gld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/43898991/adc.o: ../../adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/adc.o.ok ${OBJECTDIR}/_ext/43898991/adc.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/adc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/adc.o.d" -o ${OBJECTDIR}/_ext/43898991/adc.o ../../adc.c    
	
${OBJECTDIR}/_ext/43898991/cn.o: ../../cn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/cn.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/cn.o.ok ${OBJECTDIR}/_ext/43898991/cn.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/cn.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/cn.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/cn.o.d" -o ${OBJECTDIR}/_ext/43898991/cn.o ../../cn.c    
	
${OBJECTDIR}/_ext/43898991/crc.o: ../../crc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/crc.o.ok ${OBJECTDIR}/_ext/43898991/crc.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/crc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/crc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/crc.o.d" -o ${OBJECTDIR}/_ext/43898991/crc.o ../../crc.c    
	
${OBJECTDIR}/_ext/43898991/GasExtractSensor.o: ../../GasExtractSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o ../../GasExtractSensor.c    
	
${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o: ../../GasPlasmaFlowSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o ../../GasPlasmaFlowSensor.c    
	
${OBJECTDIR}/_ext/43898991/GasPressureSensor.o: ../../GasPressureSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o ../../GasPressureSensor.c    
	
${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o: ../../GasTransportFlowSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o ../../GasTransportFlowSensor.c    
	
${OBJECTDIR}/_ext/43898991/interruptpriority.o: ../../interruptpriority.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/interruptpriority.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/interruptpriority.o.ok ${OBJECTDIR}/_ext/43898991/interruptpriority.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/interruptpriority.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/interruptpriority.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/interruptpriority.o.d" -o ${OBJECTDIR}/_ext/43898991/interruptpriority.o ../../interruptpriority.c    
	
${OBJECTDIR}/_ext/43898991/main.o: ../../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.ok ${OBJECTDIR}/_ext/43898991/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/main.o.d" -o ${OBJECTDIR}/_ext/43898991/main.o ../../main.c    
	
${OBJECTDIR}/_ext/43898991/memory.o: ../../memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/memory.o.ok ${OBJECTDIR}/_ext/43898991/memory.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/memory.o.d" -o ${OBJECTDIR}/_ext/43898991/memory.o ../../memory.c    
	
${OBJECTDIR}/_ext/43898991/PMTSensor.o: ../../PMTSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/PMTSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/PMTSensor.o.ok ${OBJECTDIR}/_ext/43898991/PMTSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/PMTSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/PMTSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/PMTSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/PMTSensor.o ../../PMTSensor.c    
	
${OBJECTDIR}/_ext/43898991/Pneumatics.o: ../../Pneumatics.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/Pneumatics.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/Pneumatics.o.ok ${OBJECTDIR}/_ext/43898991/Pneumatics.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/Pneumatics.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/Pneumatics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/Pneumatics.o.d" -o ${OBJECTDIR}/_ext/43898991/Pneumatics.o ../../Pneumatics.c    
	
${OBJECTDIR}/_ext/43898991/pwm.o: ../../pwm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/pwm.o.ok ${OBJECTDIR}/_ext/43898991/pwm.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/pwm.o.d" -o ${OBJECTDIR}/_ext/43898991/pwm.o ../../pwm.c    
	
${OBJECTDIR}/_ext/43898991/SM1150.o: ../../SM1150.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/SM1150.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/SM1150.o.ok ${OBJECTDIR}/_ext/43898991/SM1150.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/SM1150.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/SM1150.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/SM1150.o.d" -o ${OBJECTDIR}/_ext/43898991/SM1150.o ../../SM1150.c    
	
${OBJECTDIR}/_ext/43898991/timer.o: ../../timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/timer.o.ok ${OBJECTDIR}/_ext/43898991/timer.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/timer.o.d" -o ${OBJECTDIR}/_ext/43898991/timer.o ../../timer.c    
	
${OBJECTDIR}/_ext/43898991/wake.o: ../../wake.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/wake.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/wake.o.ok ${OBJECTDIR}/_ext/43898991/wake.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/wake.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/wake.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/wake.o.d" -o ${OBJECTDIR}/_ext/43898991/wake.o ../../wake.c    
	
${OBJECTDIR}/_ext/43898991/StepMotorProba.o: ../../StepMotorProba.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.ok ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorProba.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d" -o ${OBJECTDIR}/_ext/43898991/StepMotorProba.o ../../StepMotorProba.c    
	
${OBJECTDIR}/_ext/43898991/StepMotorPMT.o: ../../StepMotorPMT.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.ok ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d" -o ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o ../../StepMotorPMT.c    
	
${OBJECTDIR}/_ext/43898991/US.o: ../../US.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/US.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/US.o.ok ${OBJECTDIR}/_ext/43898991/US.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/US.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/US.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/US.o.d" -o ${OBJECTDIR}/_ext/43898991/US.o ../../US.c    
	
${OBJECTDIR}/_ext/43898991/uart.o: ../../uart.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/uart.o.ok ${OBJECTDIR}/_ext/43898991/uart.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/uart.o.d" -o ${OBJECTDIR}/_ext/43898991/uart.o ../../uart.c    
	
${OBJECTDIR}/_ext/43898991/packet.o: ../../packet.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/packet.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/packet.o.ok ${OBJECTDIR}/_ext/43898991/packet.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/packet.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/packet.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG  -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/packet.o.d" -o ${OBJECTDIR}/_ext/43898991/packet.o ../../packet.c    
	
else
${OBJECTDIR}/_ext/43898991/adc.o: ../../adc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/adc.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/adc.o.ok ${OBJECTDIR}/_ext/43898991/adc.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/adc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/adc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/adc.o.d" -o ${OBJECTDIR}/_ext/43898991/adc.o ../../adc.c    
	
${OBJECTDIR}/_ext/43898991/cn.o: ../../cn.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/cn.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/cn.o.ok ${OBJECTDIR}/_ext/43898991/cn.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/cn.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/cn.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/cn.o.d" -o ${OBJECTDIR}/_ext/43898991/cn.o ../../cn.c    
	
${OBJECTDIR}/_ext/43898991/crc.o: ../../crc.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/crc.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/crc.o.ok ${OBJECTDIR}/_ext/43898991/crc.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/crc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/crc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/crc.o.d" -o ${OBJECTDIR}/_ext/43898991/crc.o ../../crc.c    
	
${OBJECTDIR}/_ext/43898991/GasExtractSensor.o: ../../GasExtractSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasExtractSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasExtractSensor.o ../../GasExtractSensor.c    
	
${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o: ../../GasPlasmaFlowSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasPlasmaFlowSensor.o ../../GasPlasmaFlowSensor.c    
	
${OBJECTDIR}/_ext/43898991/GasPressureSensor.o: ../../GasPressureSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasPressureSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasPressureSensor.o ../../GasPressureSensor.c    
	
${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o: ../../GasTransportFlowSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.ok ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/GasTransportFlowSensor.o ../../GasTransportFlowSensor.c    
	
${OBJECTDIR}/_ext/43898991/interruptpriority.o: ../../interruptpriority.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/interruptpriority.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/interruptpriority.o.ok ${OBJECTDIR}/_ext/43898991/interruptpriority.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/interruptpriority.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/interruptpriority.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/interruptpriority.o.d" -o ${OBJECTDIR}/_ext/43898991/interruptpriority.o ../../interruptpriority.c    
	
${OBJECTDIR}/_ext/43898991/main.o: ../../main.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o.ok ${OBJECTDIR}/_ext/43898991/main.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/main.o.d" -o ${OBJECTDIR}/_ext/43898991/main.o ../../main.c    
	
${OBJECTDIR}/_ext/43898991/memory.o: ../../memory.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/memory.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/memory.o.ok ${OBJECTDIR}/_ext/43898991/memory.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/memory.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/memory.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/memory.o.d" -o ${OBJECTDIR}/_ext/43898991/memory.o ../../memory.c    
	
${OBJECTDIR}/_ext/43898991/PMTSensor.o: ../../PMTSensor.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/PMTSensor.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/PMTSensor.o.ok ${OBJECTDIR}/_ext/43898991/PMTSensor.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/PMTSensor.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/PMTSensor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/PMTSensor.o.d" -o ${OBJECTDIR}/_ext/43898991/PMTSensor.o ../../PMTSensor.c    
	
${OBJECTDIR}/_ext/43898991/Pneumatics.o: ../../Pneumatics.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/Pneumatics.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/Pneumatics.o.ok ${OBJECTDIR}/_ext/43898991/Pneumatics.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/Pneumatics.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/Pneumatics.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/Pneumatics.o.d" -o ${OBJECTDIR}/_ext/43898991/Pneumatics.o ../../Pneumatics.c    
	
${OBJECTDIR}/_ext/43898991/pwm.o: ../../pwm.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/pwm.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/pwm.o.ok ${OBJECTDIR}/_ext/43898991/pwm.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/pwm.o.d" -o ${OBJECTDIR}/_ext/43898991/pwm.o ../../pwm.c    
	
${OBJECTDIR}/_ext/43898991/SM1150.o: ../../SM1150.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/SM1150.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/SM1150.o.ok ${OBJECTDIR}/_ext/43898991/SM1150.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/SM1150.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/SM1150.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/SM1150.o.d" -o ${OBJECTDIR}/_ext/43898991/SM1150.o ../../SM1150.c    
	
${OBJECTDIR}/_ext/43898991/timer.o: ../../timer.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/timer.o.ok ${OBJECTDIR}/_ext/43898991/timer.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/timer.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/timer.o.d" -o ${OBJECTDIR}/_ext/43898991/timer.o ../../timer.c    
	
${OBJECTDIR}/_ext/43898991/wake.o: ../../wake.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/wake.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/wake.o.ok ${OBJECTDIR}/_ext/43898991/wake.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/wake.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/wake.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/wake.o.d" -o ${OBJECTDIR}/_ext/43898991/wake.o ../../wake.c    
	
${OBJECTDIR}/_ext/43898991/StepMotorProba.o: ../../StepMotorProba.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.ok ${OBJECTDIR}/_ext/43898991/StepMotorProba.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorProba.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/StepMotorProba.o.d" -o ${OBJECTDIR}/_ext/43898991/StepMotorProba.o ../../StepMotorProba.c    
	
${OBJECTDIR}/_ext/43898991/StepMotorPMT.o: ../../StepMotorPMT.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.ok ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/StepMotorPMT.o.d" -o ${OBJECTDIR}/_ext/43898991/StepMotorPMT.o ../../StepMotorPMT.c    
	
${OBJECTDIR}/_ext/43898991/US.o: ../../US.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/US.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/US.o.ok ${OBJECTDIR}/_ext/43898991/US.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/US.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/US.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/US.o.d" -o ${OBJECTDIR}/_ext/43898991/US.o ../../US.c    
	
${OBJECTDIR}/_ext/43898991/uart.o: ../../uart.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/uart.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/uart.o.ok ${OBJECTDIR}/_ext/43898991/uart.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/uart.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/uart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/uart.o.d" -o ${OBJECTDIR}/_ext/43898991/uart.o ../../uart.c    
	
${OBJECTDIR}/_ext/43898991/packet.o: ../../packet.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/_ext/43898991" 
	@${RM} ${OBJECTDIR}/_ext/43898991/packet.o.d 
	@${RM} ${OBJECTDIR}/_ext/43898991/packet.o.ok ${OBJECTDIR}/_ext/43898991/packet.o.err 
	@${RM} ${OBJECTDIR}/_ext/43898991/packet.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/43898991/packet.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -omf=elf -x c -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/_ext/43898991/packet.o.d" -o ${OBJECTDIR}/_ext/43898991/packet.o ../../packet.c    
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../../APP\ 24FJ128GA106\ IVT\ remap.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG  -o dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../../APP\ 24FJ128GA106\ IVT\ remap.gld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -omf=elf -mcpu=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}         -Wl,--defsym=__MPLAB_BUILD=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION)
	${MP_CC_DIR}\\pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/Spectrometer2.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -omf=elf
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
