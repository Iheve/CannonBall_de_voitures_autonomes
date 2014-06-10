DIADIR=dia
IMGDIR=img
TEXDIR=tex
PDFDIR=pdf
COMPDIR=.compil

DIA=$(wildcard $(DIADIR)/*.dia)
IMG=$(addprefix $(IMGDIR)/, $(notdir $(DIA:%.dia=%.png)))
DEP=$(TEXDIR)/preambule.tex
TEX=$(filter-out $(DEP), $(wildcard $(TEXDIR)/*.tex))
FILES=$(addprefix $(PDFDIR)/, $(notdir $(TEX:%.tex=%.pdf)))

DIRGUARD=mkdir -p $(COMPDIR) $(IMGDIR) $(PDFDIR)
LATEX=pdflatex -output-directory=$(COMPDIR) $<

.SECONDARY: $(IMG)

all: $(FILES)

echo:
	@echo $(IMG)
	@echo $(DIA)
	@echo $(DEP)
	@echo $(TEX)
	@echo $(FILES)


$(IMGDIR)/%.png: $(DIADIR)/%.dia
	$(DIRGUARD)
ifeq ($(TUX), true)
	cp tux.png $@
else
	dia -e $@ -t png $<
endif

$(IMGDIR)/%.pdf: $(DIADIR)/%.dia
	$(DIRGUARD)
	dia -s 800x -e $@ -t pdf $<

$(PDFDIR)/%.pdf: $(TEXDIR)/%.tex $(DEP) $(IMG)
	$(DIRGUARD)
	$(LATEX) -draft && $(LATEX)
	mv $(COMPDIR)/$(notdir $@) $@

view: $(FILES)
	@if ! okular $^ 2> /dev/null; then if ! zathura $^ 2> /dev/null; then if ! open $^ 2> /dev/null; then evince $^ 2> /dev/null; fi fi fi &

clean:
	rm -rf $(COMPDIR) $(IMGDIR)

realclean: clean
	rm -rf $(PDFDIR)
