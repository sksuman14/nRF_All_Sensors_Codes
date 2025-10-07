cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(Step_Motor_28BYJ-48)

target_sources(app PRIVATE src/main.c)
