TEXDIR=tex
TEX=$(shell cd tex && find . -name "*.tex")
PDF=$(TEX:%.tex=%.pdf)

all: $(PDF)

%.pdf: $(TEXDIR)/%.tex
	pdflatex $^

.PHONY: clean
clean:
	rm -rf *.toc *.aux *.log *.out 2> /dev/null

proper:
	rm -rf *.pdf 2> /dev/null
