pdflatex:= $(shell which latexmk)

all: report

report:
	$(pdflatex) --pdf report

clean:
	$(pdflatex) --c report
	-@/bin/rm -f report.bbl
	-@/usr/bin/find . -name '*~' -exec rm -f {} \;

distclean: clean
	$(pdflatex) --C report

