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
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=dspic-libs/adc.c dspic-libs/buzzer.c dspic-libs/keyboard.c dspic-libs/LCDlib.c dspic-libs/leds.c dspic-libs/timer.c dspic-libs/delay.s dspic-libs/libCAN.c dspic-libs/terminal.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/dspic-libs/adc.o ${OBJECTDIR}/dspic-libs/buzzer.o ${OBJECTDIR}/dspic-libs/keyboard.o ${OBJECTDIR}/dspic-libs/LCDlib.o ${OBJECTDIR}/dspic-libs/leds.o ${OBJECTDIR}/dspic-libs/timer.o ${OBJECTDIR}/dspic-libs/delay.o ${OBJECTDIR}/dspic-libs/libCAN.o ${OBJECTDIR}/dspic-libs/terminal.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/dspic-libs/adc.o.d ${OBJECTDIR}/dspic-libs/buzzer.o.d ${OBJECTDIR}/dspic-libs/keyboard.o.d ${OBJECTDIR}/dspic-libs/LCDlib.o.d ${OBJECTDIR}/dspic-libs/leds.o.d ${OBJECTDIR}/dspic-libs/timer.o.d ${OBJECTDIR}/dspic-libs/delay.o.d ${OBJECTDIR}/dspic-libs/libCAN.o.d ${OBJECTDIR}/dspic-libs/terminal.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/dspic-libs/adc.o ${OBJECTDIR}/dspic-libs/buzzer.o ${OBJECTDIR}/dspic-libs/keyboard.o ${OBJECTDIR}/dspic-libs/LCDlib.o ${OBJECTDIR}/dspic-libs/leds.o ${OBJECTDIR}/dspic-libs/timer.o ${OBJECTDIR}/dspic-libs/delay.o ${OBJECTDIR}/dspic-libs/libCAN.o ${OBJECTDIR}/dspic-libs/terminal.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=dspic-libs/adc.c dspic-libs/buzzer.c dspic-libs/keyboard.c dspic-libs/LCDlib.c dspic-libs/leds.c dspic-libs/timer.c dspic-libs/delay.s dspic-libs/libCAN.c dspic-libs/terminal.c main.c


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
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=30F4011
MP_LINKER_FILE_OPTION=,--script=p30F4011.gld
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/dspic-libs/adc.o: dspic-libs/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/adc.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/adc.c  -o ${OBJECTDIR}/dspic-libs/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/adc.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/buzzer.o: dspic-libs/buzzer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/buzzer.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/buzzer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/buzzer.c  -o ${OBJECTDIR}/dspic-libs/buzzer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/buzzer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/buzzer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/keyboard.o: dspic-libs/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/keyboard.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/keyboard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/keyboard.c  -o ${OBJECTDIR}/dspic-libs/keyboard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/keyboard.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/keyboard.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/LCDlib.o: dspic-libs/LCDlib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/LCDlib.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/LCDlib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/LCDlib.c  -o ${OBJECTDIR}/dspic-libs/LCDlib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/LCDlib.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/LCDlib.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/leds.o: dspic-libs/leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/leds.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/leds.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/leds.c  -o ${OBJECTDIR}/dspic-libs/leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/leds.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/timer.o: dspic-libs/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/timer.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/timer.c  -o ${OBJECTDIR}/dspic-libs/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/timer.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/libCAN.o: dspic-libs/libCAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/libCAN.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/libCAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/libCAN.c  -o ${OBJECTDIR}/dspic-libs/libCAN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/libCAN.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/libCAN.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/terminal.o: dspic-libs/terminal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/terminal.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/terminal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/terminal.c  -o ${OBJECTDIR}/dspic-libs/terminal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/terminal.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/terminal.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"      -g -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1    -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/dspic-libs/adc.o: dspic-libs/adc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/adc.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/adc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/adc.c  -o ${OBJECTDIR}/dspic-libs/adc.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/adc.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/adc.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/buzzer.o: dspic-libs/buzzer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/buzzer.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/buzzer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/buzzer.c  -o ${OBJECTDIR}/dspic-libs/buzzer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/buzzer.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/buzzer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/keyboard.o: dspic-libs/keyboard.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/keyboard.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/keyboard.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/keyboard.c  -o ${OBJECTDIR}/dspic-libs/keyboard.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/keyboard.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/keyboard.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/LCDlib.o: dspic-libs/LCDlib.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/LCDlib.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/LCDlib.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/LCDlib.c  -o ${OBJECTDIR}/dspic-libs/LCDlib.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/LCDlib.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/LCDlib.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/leds.o: dspic-libs/leds.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/leds.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/leds.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/leds.c  -o ${OBJECTDIR}/dspic-libs/leds.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/leds.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/leds.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/timer.o: dspic-libs/timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/timer.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/timer.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/timer.c  -o ${OBJECTDIR}/dspic-libs/timer.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/timer.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/timer.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/libCAN.o: dspic-libs/libCAN.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/libCAN.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/libCAN.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/libCAN.c  -o ${OBJECTDIR}/dspic-libs/libCAN.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/libCAN.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/libCAN.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/dspic-libs/terminal.o: dspic-libs/terminal.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/terminal.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/terminal.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  dspic-libs/terminal.c  -o ${OBJECTDIR}/dspic-libs/terminal.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/dspic-libs/terminal.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/terminal.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/main.o: main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR} 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE)  main.c  -o ${OBJECTDIR}/main.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -MMD -MF "${OBJECTDIR}/main.o.d"        -g -omf=elf -O0 -msmart-io=1 -Wall -msfr-warn=off
	@${FIXDEPS} "${OBJECTDIR}/main.o.d" $(SILENT)  -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/dspic-libs/delay.o: dspic-libs/delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/delay.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/delay.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  dspic-libs/delay.s  -o ${OBJECTDIR}/dspic-libs/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf -Wa,-MD,"${OBJECTDIR}/dspic-libs/delay.o.d",--defsym=__MPLAB_BUILD=1,--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/delay.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/dspic-libs/delay.o: dspic-libs/delay.s  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/dspic-libs 
	@${RM} ${OBJECTDIR}/dspic-libs/delay.o.d 
	@${RM} ${OBJECTDIR}/dspic-libs/delay.o 
	${MP_CC} $(MP_EXTRA_AS_PRE)  dspic-libs/delay.s  -o ${OBJECTDIR}/dspic-libs/delay.o  -c -mcpu=$(MP_PROCESSOR_OPTION)  -omf=elf -Wa,-MD,"${OBJECTDIR}/dspic-libs/delay.o.d",--defsym=__MPLAB_BUILD=1,-g,--no-relax$(MP_EXTRA_AS_POST)
	@${FIXDEPS} "${OBJECTDIR}/dspic-libs/delay.o.d"  $(SILENT)  -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemblePreproc
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1  -omf=elf  -mreserve=data@0x800:0x81F -mreserve=data@0x820:0x821 -mreserve=data@0x822:0x823 -mreserve=data@0x824:0x84F   -Wl,,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_ICD3=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -o dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -mcpu=$(MP_PROCESSOR_OPTION)        -omf=elf -Wl,,--defsym=__MPLAB_BUILD=1,$(MP_LINKER_FILE_OPTION),--stack=16,--check-sections,--data-init,--pack-data,--handles,--isr,--no-gc-sections,--fill-upper=0,--stackguard=16,--no-force-link,--smart-io,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--report-mem$(MP_EXTRA_LD_POST) 
	${MP_CC_DIR}\\xc16-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/elevator-simulator.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} -a  -omf=elf  
	
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
