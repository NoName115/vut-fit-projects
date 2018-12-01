NAME=proj2

all: clean do

do: $(NAME).tex
	latex $(NAME).tex
	bibtex $(NAME) || true
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf $(NAME).ps

clean:
	rm -f $(NAME).dvi $(NAME).log $(NAME).aux $(NAME).ps $(NAME).pdf $(NAME).bbl $(NAME).blg
