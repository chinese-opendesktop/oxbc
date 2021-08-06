PKGNAME := oxbc
PROGS := bc-simon-cpp bc-robert-c bc-john-js bc-kevin-js bc-william-sh bc-simon-java bc-robert-python
.PHONY: all install

all: $(PROGS)

bc-simon-cpp: bc-simon.cpp
	-$(CXX) -std=c++11 -o $@ $<
	
bc-robert-c: bc-robert.c
	-$(CC) -lm -o $@ $< 
	
bc-john-js: bc-john.js
	cp $< $@ 
	
bc-kevin-js: bc-kevin.js
	cp $< $@ 

bc-william-sh: bc-william.sh
	cp $< $@ 

bc-simon-java: bc_simon.java
	-javac bc_simon.java && cp bc-simon-java.sh $@

bc-robert-python: bc-robert.py
	cp $< $@ 

install: $(PROGS)
	install -d $(DESTDIR)/usr/bin/
	install -m 755 oxbc.sh $(DESTDIR)/usr/bin/oxbc	
	install -d $(DESTDIR)/usr/libexec/$(PKGNAME)
	-install -m 755 $(PROGS) $(DESTDIR)/usr/libexec/$(PKGNAME)
	-install -m 755 *.class $(DESTDIR)/usr/libexec/$(PKGNAME)
	install -m 644 TESTS $(DESTDIR)/usr/libexec/$(PKGNAME)
