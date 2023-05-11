CC		=	clang
CXX     =  clang++
SDIR	:=	.
IDIR	:=	-I.
CFLAGS	:=	$(IDIR) -O3 -g
CXXFLAGS	:=	$(IDIR) -O3 --std=c++20 -g
LFLAGS	:=	-lSDL2 -g
ODIR	:=	build
CFILES	:=  main.cpp x86.lle.c x86.cpu.c
OBJS	:=	$(ODIR)/main.o $(ODIR)/x86.lle.o $(ODIR)/x86.cpu.o

bin: $(ODIR) $(OBJS)
	$(CXX) $(ODIR)/*.o -o x86.bin $(LFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)
$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean

clean:
	rm -f x86.bin $(ODIR)/*.o
	rm -rf $(ODIR)