
# dependency files
TOOLS_DEPS := tools/mkfs.d\
				tools/mkfnt.d

# all generated files
TOOLS_CLEAN := tools/mkfs\
			tools/mkfs.o\
			tools/mkfnt\
			tools/mkfnt.o\
			$(TOOLS_DEPS)

# flags
TOOLS_CPPFLAGS := -iquote include

# mkfs
tools/mkfs: tools/mkfs.o
	$(CC) $(LDFLAGS) $< -o $@

tools/mkfnt: tools/mkfnt.o tools/fntsrc.txt
	$(CXX) $(LDFLAGS)-o $@ $<

# build object files from c files
tools/%.o: tools/%.c
	$(CC) -c $(CPPFLAGS) $(TOOLS_CPPFLAGS) $(CFLAGS) $(TOOLS_CLFAGS) -o $@ $<

# build dependency files form c files
tools/%.d: tools/%.c
	$(CC) $(CPPFLAGS) $(TOOLS_CPPFLAGS) $(CFLAGS) $(TOOLS_CFLAGS) \
	  -M -MG $< -MF $@ -MT $@ -MT $(<:.c=.o)

tools/%.o: tools/%.cpp
	$(CXX) -c $(CPPFLAGS) $(TOOLS_CPPFLAGS) $(CFLAGS) $(TOOLS_CLFAGS) -o $@ $<

# build dependency files form c files
tools/%.d: tools/%.cpp
	$(CXX) $(CPPFLAGS) $(TOOLS_CPPFLAGS) $(CFLAGS) $(TOOLS_CFLAGS) \
	  -M -MG $< -MF $@ -MT $@ -MT $(<:.cpp=.o)


