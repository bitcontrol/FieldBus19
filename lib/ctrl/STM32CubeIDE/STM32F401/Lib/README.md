# Controller Library Release Build

Link this library to your project.

**MCU GCC Assembler Options**<br>
```
-mcpu=cortex-m4
-c
-x assembler-with-cpp
--specs=nano.specs
-mfpu=fpv4-sp-d16
-mfloat-abi=hard
-mthumb
```

**MCU GCC Compiler Options**<br>
```
-mcpu=cortex-m4
-std=gnu11
-DSTM32F401xx
-DSTM32F401xE
-c
-I... // All necessary includes come here
-Os
-ffunction-sections
-fdata-sections
-Wall
-fstack-usage
-fcyclomatic-complexity
--specs=nano.specs
-mfpu=fpv4-sp-d16
-mfloat-abi=hard
-mthumb
```
