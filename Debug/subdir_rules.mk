################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Clock.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/Clock.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

LaunchPad.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/LaunchPad.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SPI.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/SPI.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

ST7735.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/ST7735.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

SlidePot.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/SlidePot.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

TExaS.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/TExaS.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

Timer.o: /Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/inc/Timer.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs2000/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang" -c -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"/Applications/ti/mspm0_sdk_2_03_00_07/source" -D__MSPM0G3507__ -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"/Users/adipu/Library/CloudStorage/OneDrive-TheUniversityofTexasatAustin/Freshman/319H/MSPM0_ValvanoWare/ECE319K_Lab9H/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


