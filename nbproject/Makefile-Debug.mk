#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-none-eabi-gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/cmsis_boot/startup/startup_stm32f4xx.o \
	${OBJECTDIR}/cmsis_boot/system_stm32f4xx.o \
	${OBJECTDIR}/cmsis_lib/source/misc.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_gpio.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwm.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rcc.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_tim.o \
	${OBJECTDIR}/lcd/lcd.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-mcpu=cortex-m4 -mthumb -Wall -ffunction-sections -g -O0 -DSTM32F407VG -DSTM32F4XX -DUSE_STDPERIPH_DRIVER -D__ASSEMBLY__

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32pwm

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32pwm: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32pwm ${OBJECTFILES} ${LDLIBSOPTIONS} -mcpu=cortex-m4 -mthumb -g -nostartfiles -DSTM32F407VG -DSTM32F4XX -TSTM32F407VG_FLASH.ld -Wl,--gc-sections -g -o main.elf && arm-none-eabi-objcopy -Oihex main.elf main.hex

${OBJECTDIR}/cmsis_boot/startup/startup_stm32f4xx.o: cmsis_boot/startup/startup_stm32f4xx.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_boot/startup
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_boot/startup/startup_stm32f4xx.o cmsis_boot/startup/startup_stm32f4xx.c

${OBJECTDIR}/cmsis_boot/system_stm32f4xx.o: cmsis_boot/system_stm32f4xx.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_boot
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_boot/system_stm32f4xx.o cmsis_boot/system_stm32f4xx.c

${OBJECTDIR}/cmsis_lib/source/misc.o: cmsis_lib/source/misc.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/misc.o cmsis_lib/source/misc.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_gpio.o: cmsis_lib/source/stm32f4xx_gpio.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_gpio.o cmsis_lib/source/stm32f4xx_gpio.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwm.o: cmsis_lib/source/stm32f4xx_pwm.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwm.o cmsis_lib/source/stm32f4xx_pwm.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rcc.o: cmsis_lib/source/stm32f4xx_rcc.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rcc.o cmsis_lib/source/stm32f4xx_rcc.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_tim.o: cmsis_lib/source/stm32f4xx_tim.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_tim.o cmsis_lib/source/stm32f4xx_tim.c

${OBJECTDIR}/lcd/lcd.o: lcd/lcd.c 
	${MKDIR} -p ${OBJECTDIR}/lcd
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lcd/lcd.o lcd/lcd.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32pwm

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
