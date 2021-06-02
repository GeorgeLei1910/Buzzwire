#!/bin/bash

g++ -c SFMLApplication/main.cpp
g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system