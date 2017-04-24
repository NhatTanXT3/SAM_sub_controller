################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
SAM.obj: ../SAM.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="SAM.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

mainSubController.obj: ../mainSubController.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="mainSubController.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

myFIFO.obj: ../myFIFO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="myFIFO.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

myIO.obj: ../myIO.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="myIO.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

myRS485.obj: ../myRS485.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="myRS485.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

mySerial.obj: ../mySerial.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="mySerial.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

myTimer.obj: ../myTimer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="myTimer.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

numManipulate.obj: ../numManipulate.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="numManipulate.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

ringbuf.obj: /opt/ti/TivaWare_C_Series-2.1.0.12573/utils/ringbuf.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="ringbuf.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

serial.obj: ../serial.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="serial.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

tm4c123gh6pm_startup_ccs.obj: ../tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --include_path="/opt/ti/ccs710/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="tm4c123gh6pm_startup_ccs.d" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


