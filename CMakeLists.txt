cmake_minimum_required(VERSION 3.20)
project(C_Practice_cs50_2024 C)

set(CMAKE_C_STANDARD 11)

# Target 1: Your Hello World Program
add_executable(hello_program hello.c cs50.c)

# Target 2: Your Compare Program
add_executable(compare_program compare.c cs50.c)

# Target 3: Your Agree Program
add_executable(agree_program agree.c cs50.c)

# Target 4: Your Meow Program
add_executable(meow_program meow.c cs50.c)

# Target 5: Your Calculator Program
add_executable(calculator_program calculator.c cs50.c)