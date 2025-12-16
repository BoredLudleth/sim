# Simulator
### Дисклеймер
Всем привет!
Данный проект выполнен в учебных целях в рамках курса "Симуляция компьютерный систем". 

Целью данного проекта является написание симулятора.

## Зависимости

## Компиляция
```
cd sim/
make -B build -S . -DENABLE_CACHE=OFF -DENABLE_MMU=ON
cmake --build build
```
При сборке проекта можно использовать флаги 

Для запуска проекта требуется:

```
./build/riscv-simulator ./examples/queens8.elf
```


По сути, работа симулятора сводится к повторению следующих действий:

0. Прочитать ELF-файл и загрузить его в память.
1. Извлечь инструкцию для исполнения
2. Декодировать инструкцию
3. Исполнить инструкцию
4. Записать результат
5. Продвинуть pc к следующей инструкции (шаг 1)

## How to create elf-file?
```
riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -ffreestanding -Ttext=0x80000000 -Wl,-Map=output.map -o examples/fibonacci.elf examples/fibonacci.c
```

