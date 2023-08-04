CXX=g++
RM = del
EXTENSION = .exe

relics: relics.cpp
	g++ relics.cpp -o relics$(EXTENSION)
clean:
	$(RM) relics$(EXTENSION)
