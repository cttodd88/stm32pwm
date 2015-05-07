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
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Config/GUIConf.o \
	${OBJECTDIR}/Config/GUIDRV_Template.o \
	${OBJECTDIR}/Config/LCDConf_FlexColor_Template.o \
	${OBJECTDIR}/Config/SIMConf.o \
	${OBJECTDIR}/cmsis_boot/startup/startup_stm32f4xx.o \
	${OBJECTDIR}/cmsis_boot/system_stm32f4xx.o \
	${OBJECTDIR}/cmsis_lib/source/misc.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_exti.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_fsmc.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_gpio.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwm.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwr.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rcc.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rtc.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_spi.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_syscfg.o \
	${OBJECTDIR}/cmsis_lib/source/stm32f4xx_tim.o \
	${OBJECTDIR}/delays.o \
	${OBJECTDIR}/img/sentinelLogo1.o \
	${OBJECTDIR}/img/sentinelLogo2.o \
	${OBJECTDIR}/img/sentinelLogoBW.o \
	${OBJECTDIR}/lcd/lcd.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/rtc.o \
	${OBJECTDIR}/sentinelLogo.o \
	${OBJECTDIR}/syscalls/syscalls.o \
	${OBJECTDIR}/user/GUI_X_Touch.o \
	${OBJECTDIR}/user/LCD_STM32F4.o \
	${OBJECTDIR}/user/PlianHourHandbw.o \
	${OBJECTDIR}/user/gui_x.o \
	${OBJECTDIR}/user/pwm.o \
	${OBJECTDIR}/user/stm32f4xx_it.o \
	${OBJECTDIR}/user/ts.o


# C Compiler Flags
CFLAGS=

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
	${LINK.c} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/stm32pwm ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/Config/GUIConf.o: Config/GUIConf.c 
	${MKDIR} -p ${OBJECTDIR}/Config
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config/GUIConf.o Config/GUIConf.c

${OBJECTDIR}/Config/GUIDRV_Template.o: Config/GUIDRV_Template.c 
	${MKDIR} -p ${OBJECTDIR}/Config
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config/GUIDRV_Template.o Config/GUIDRV_Template.c

${OBJECTDIR}/Config/LCDConf_FlexColor_Template.o: Config/LCDConf_FlexColor_Template.c 
	${MKDIR} -p ${OBJECTDIR}/Config
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config/LCDConf_FlexColor_Template.o Config/LCDConf_FlexColor_Template.c

${OBJECTDIR}/Config/SIMConf.o: Config/SIMConf.c 
	${MKDIR} -p ${OBJECTDIR}/Config
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Config/SIMConf.o Config/SIMConf.c

${OBJECTDIR}/cmsis_boot/startup/startup_stm32f4xx.o: cmsis_boot/startup/startup_stm32f4xx.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_boot/startup
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_boot/startup/startup_stm32f4xx.o cmsis_boot/startup/startup_stm32f4xx.c

${OBJECTDIR}/cmsis_boot/system_stm32f4xx.o: cmsis_boot/system_stm32f4xx.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_boot
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_boot/system_stm32f4xx.o cmsis_boot/system_stm32f4xx.c

${OBJECTDIR}/cmsis_lib/source/misc.o: cmsis_lib/source/misc.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/misc.o cmsis_lib/source/misc.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_exti.o: cmsis_lib/source/stm32f4xx_exti.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_exti.o cmsis_lib/source/stm32f4xx_exti.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_fsmc.o: cmsis_lib/source/stm32f4xx_fsmc.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_fsmc.o cmsis_lib/source/stm32f4xx_fsmc.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_gpio.o: cmsis_lib/source/stm32f4xx_gpio.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_gpio.o cmsis_lib/source/stm32f4xx_gpio.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwm.o: cmsis_lib/source/stm32f4xx_pwm.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwm.o cmsis_lib/source/stm32f4xx_pwm.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwr.o: cmsis_lib/source/stm32f4xx_pwr.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_pwr.o cmsis_lib/source/stm32f4xx_pwr.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rcc.o: cmsis_lib/source/stm32f4xx_rcc.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rcc.o cmsis_lib/source/stm32f4xx_rcc.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rtc.o: cmsis_lib/source/stm32f4xx_rtc.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_rtc.o cmsis_lib/source/stm32f4xx_rtc.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_spi.o: cmsis_lib/source/stm32f4xx_spi.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_spi.o cmsis_lib/source/stm32f4xx_spi.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_syscfg.o: cmsis_lib/source/stm32f4xx_syscfg.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_syscfg.o cmsis_lib/source/stm32f4xx_syscfg.c

${OBJECTDIR}/cmsis_lib/source/stm32f4xx_tim.o: cmsis_lib/source/stm32f4xx_tim.c 
	${MKDIR} -p ${OBJECTDIR}/cmsis_lib/source
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/cmsis_lib/source/stm32f4xx_tim.o cmsis_lib/source/stm32f4xx_tim.c

${OBJECTDIR}/delays.o: delays.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/delays.o delays.c

${OBJECTDIR}/img/sentinelLogo1.o: img/sentinelLogo1.c 
	${MKDIR} -p ${OBJECTDIR}/img
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/img/sentinelLogo1.o img/sentinelLogo1.c

${OBJECTDIR}/img/sentinelLogo2.o: img/sentinelLogo2.c 
	${MKDIR} -p ${OBJECTDIR}/img
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/img/sentinelLogo2.o img/sentinelLogo2.c

${OBJECTDIR}/img/sentinelLogoBW.o: img/sentinelLogoBW.c 
	${MKDIR} -p ${OBJECTDIR}/img
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/img/sentinelLogoBW.o img/sentinelLogoBW.c

${OBJECTDIR}/lcd/lcd.o: lcd/lcd.c 
	${MKDIR} -p ${OBJECTDIR}/lcd
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/lcd/lcd.o lcd/lcd.c

${OBJECTDIR}/main.o: main.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.c

${OBJECTDIR}/rtc.o: rtc.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/rtc.o rtc.c

${OBJECTDIR}/sentinelLogo.o: sentinelLogo.c 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/sentinelLogo.o sentinelLogo.c

${OBJECTDIR}/syscalls/syscalls.o: syscalls/syscalls.c 
	${MKDIR} -p ${OBJECTDIR}/syscalls
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/syscalls/syscalls.o syscalls/syscalls.c

${OBJECTDIR}/user/GUI_X_Touch.o: user/GUI_X_Touch.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/GUI_X_Touch.o user/GUI_X_Touch.c

${OBJECTDIR}/user/LCD_STM32F4.o: user/LCD_STM32F4.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/LCD_STM32F4.o user/LCD_STM32F4.c

${OBJECTDIR}/user/PlianHourHandbw.o: user/PlianHourHandbw.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/PlianHourHandbw.o user/PlianHourHandbw.c

${OBJECTDIR}/user/gui_x.o: user/gui_x.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/gui_x.o user/gui_x.c

${OBJECTDIR}/user/pwm.o: user/pwm.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/pwm.o user/pwm.c

${OBJECTDIR}/user/stm32f4xx_it.o: user/stm32f4xx_it.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/stm32f4xx_it.o user/stm32f4xx_it.c

${OBJECTDIR}/user/ts.o: user/ts.c 
	${MKDIR} -p ${OBJECTDIR}/user
	${RM} "$@.d"
	$(COMPILE.c) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/user/ts.o user/ts.c

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
